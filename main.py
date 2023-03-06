import serial
import speech_recognition as sr
import openai

com_port = 'COM16'

ser = serial.Serial('com_port', 115200)
r = sr.Recognizer()

x = 0

openai.api_key = "YOUR_OPENAI_KEY"

def generate_text(prompt):
    response = openai.Completion.create(
      engine="text-davinci-003",
      prompt=prompt,
      max_tokens=1024,
      n=1,
      stop=None,
      temperature=0.5
    )

    return response.choices[0].text

while True:
    serial_read_string = ser.readline()
    serial_read_string = serial_read_string.decode(encoding='utf-8')
    serial_read_string = serial_read_string.replace("\n", "")

    if serial_read_string.startswith("btn") == 1:
        serial_read_string = serial_read_string.strip("btn: ")

    print(serial_read_string)

    if serial_read_string.startswith("0") == 1:

        recording_text = "msg: Recording"
        ser.write(recording_text.encode())

        with sr.Microphone() as source:
            r.adjust_for_ambient_noise(source)
            print("Say something!")
            audio = r.listen(source)

        try:
            text = r.recognize_google(audio)
            print("You said: {}".format(text))
        except sr.UnknownValueError:
            text = ("Sorry, could not understand audio.")
        except sr.RequestError as e:
            text = ("Error: {}".format(e))

        ai_response = generate_text(text)
        print(ai_response)
        ai_response = "msg:" + ai_response
        ser.write(ai_response.encode())
