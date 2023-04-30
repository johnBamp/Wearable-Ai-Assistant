from flask import Flask, request, jsonify
import os

app = Flask(__name__)
UPLOAD_FOLDER = 'uploaded_files'
ALLOWED_EXTENSIONS = {'wav'}

if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'X-File-Name' not in request.headers:
        return jsonify({'error': 'Missing X-File-Name header'}), 400

    filename = request.headers['X-File-Name']
    if filename == '':
        return jsonify({'error': 'No selected file'}), 400
    if not allowed_file(filename):
        return jsonify({'error': 'Invalid file type'}), 400

    file_data = request.get_data()
    file_path = os.path.join(UPLOAD_FOLDER, filename)
    with open(file_path, 'wb') as f:
        f.write(file_data)

    return jsonify({'message': 'File uploaded successfully'}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
