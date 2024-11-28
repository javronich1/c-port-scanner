from flask import Flask, request, jsonify, render_template
import subprocess

app = Flask(__name__, static_folder='static', template_folder='templates')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/scan', methods=['POST'])
def scan():
    # Parse request data
    data = request.json
    ip = data.get('ip')
    start_port = data.get('start_port')
    end_port = data.get('end_port')

    if not ip or not start_port or not end_port:
        return jsonify({"error": "Missing required fields"}), 400

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