from flask import Flask, request, jsonify
import os
import struct
import wave
import numpy as np
import os
import openai

openai.api_key = "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"

def change_pitch(data, pitch_factor):
    indices = np.round(np.arange(0, len(data), pitch_factor))
    indices = indices[indices < len(data)].astype(int)
    return data[indices]

def change_volume(data, volume_factor):
    return np.clip(data * volume_factor, -32768, 32767).astype(np.int16)

input_file = "C:\\Users\\John\\PycharmProjects\\pythonProject9\\uploaded_files\\audio.raw"
#input_file = "G:\\audio.raw"

output_file = 'audio.wav'
sample_rate = 16000
pitch_factor = 0.6 # Change the pitch. Use values <1.0 for lower pitch, >1.0 for higher pitch
volume_factor = 1.5  # Change the volume. Use values <1.0 for lower volume, >1.0 for higher volume

app = Flask(__name__)
UPLOAD_FOLDER = 'uploaded_files'

if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'X-File-Name' not in request.headers:
        return jsonify({'error': 'Missing X-File-Name header'}), 400

    filename = request.headers['X-File-Name']
    if filename == '':
        return jsonify({'error': 'No selected file'}), 400

    file_data = request.get_data()
    file_path = os.path.join(UPLOAD_FOLDER, filename)
    with open(file_path, 'wb') as f:
        f.write(file_data)

    with open(file_path, 'rb') as f:
        raw_data = f.read()

    # Convert raw data to integers
    data = struct.unpack('<' + 'h' * (len(raw_data) // 2), raw_data)

    # Convert integers to numpy array
    data_np = np.array(data, dtype=np.int16)

    # Change pitch
    data_np = change_pitch(data_np, pitch_factor)

    # Change volume
    data_np = change_volume(data_np, volume_factor)

    # Save as WAV file
    with wave.open(output_file, 'wb') as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(sample_rate)
        wav_file.writeframes(data_np.tobytes())

    audio_file = open("audio.wav", "rb")
    transcript = openai.Audio.transcribe("whisper-1", audio_file)
    transcript = transcript["text"]

    completion = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[
            {"role": "user", "content": transcript}
        ]
    )

    result = completion.choices[0]["message"]["content"]
    print("Transcript: ",transcript, "Result: ",result)

    return jsonify({'message': result}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)

