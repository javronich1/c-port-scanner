from flask import Flask, request, jsonify, render_template
import subprocess
import socket
import re

app = Flask(__name__, static_folder='static', template_folder='templates')

def is_valid_domain(domain):
    pattern = r'^(?:[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?\.)+[a-zA-Z]{2,}$'
    return bool(re.match(pattern, domain))

def is_valid_ip(ip):
    try:
        socket.inet_aton(ip)
        return True
    except socket.error:
        return False

def resolve_domain(domain):
    try:
        return socket.gethostbyname(domain)
    except socket.gaierror:
        return None

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/scan', methods=['POST'])
def scan():
    # Parse request data
    data = request.json
    target = data.get('target')
    start_port = data.get('start_port')
    end_port = data.get('end_port')

    if not target or not start_port or not end_port:
        return jsonify({"error": "Missing required fields"}), 400

    # Check if target is IP or domain
    if is_valid_ip(target):
        ip = target
    elif is_valid_domain(target):
        ip = resolve_domain(target)
        if not ip:
            return jsonify({"error": "Could not resolve domain name"}), 400
    else:
        return jsonify({"error": "Invalid IP address or domain name"}), 400

    # Run the C program
    try:
        result = subprocess.run(
            ['./project_port_scanner', ip, str(start_port), str(end_port)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
        output = result.stdout.decode()
        error = result.stderr.decode()

        if result.returncode != 0:
            return jsonify({"error": error}), 500

        return jsonify({"output": output})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)