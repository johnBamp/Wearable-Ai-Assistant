import struct
import wave
import numpy as np

def change_pitch(data, pitch_factor):
    indices = np.round(np.arange(0, len(data), pitch_factor))
    indices = indices[indices < len(data)].astype(int)
    return data[indices]

def change_volume(data, volume_factor):
    return np.clip(data * volume_factor, -32768, 32767).astype(np.int16)

input_file = 'audio.raw'
output_file = 'audio.wav'
sample_rate = 16000
pitch_factor = 1.0  # Change the pitch. Use values <1.0 for lower pitch, >1.0 for higher pitch
volume_factor = 1.0  # Change the volume. Use values <1.0 for lower volume, >1.0 for higher volume

# Read raw data from the file
with open(input_file, 'rb') as f:
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
