import sounddevice as sd
from scipy.io.wavfile import write
import speech_recognition as sr
from gtts import gTTS
import io
import tempfile
import pygame
from os import remove, name
from sys import exit
from threading import Thread, Lock
from langchain_ollama import OllamaLLM
from langchain_core.prompts import ChatPromptTemplate

class speechManager:
    def __init__(self) -> None:
        self.t = Thread(target=self.runSpeechManager, daemon=False)
        self.runningFlag = True
        self.lock = Lock()  # Lock oluşturuluyor
        print("Ses yöneticisi başlatıldı")

        self.context = ""
        self.template = """
        Answer the question below.
        Here is the conversation history: {context}
        Question: {question}
        Answer: 
        """
        self.model = OllamaLLM(model="llama3.1:latest")
        self.promt = ChatPromptTemplate.from_template(self.template)
        self.chain = self.promt | self.model
        print("Yapay zeka dil modeli başlatıldı.")

    def record_audio(self, duration=5, filename= "output.wav", samplerate=44100):
        with self.lock: 
            print("Kaydediliyor...")
            audio_data = sd.rec(int(duration * samplerate), samplerate=samplerate, channels=1, dtype='int16')
            sd.wait() 
            print("Kayıt bitti.")
            
            write(filename, samplerate, audio_data)
            print(f"Ses {filename} olarak kaydedildi.")

    def audio_to_text(self, filename):
        with self.lock:  # Lock ile ses dosyasını yazıya çevirme işlemi
            recognizer = sr.Recognizer()
            with sr.AudioFile(filename) as source:
                print("Ses dosyası işleniyor...")
                audio = recognizer.record(source)  
                
            try:
                text = recognizer.recognize_google(audio, language="tr-TR")  
                print("Metin tanındı:")
                print("Komut:", text)
                return text
            except sr.UnknownValueError:
                print("Ses anlaşılamadı.")
                return None
            except sr.RequestError as e:
                print(f"Google API hizmetine erişilemedi: {e}")

    def text_to_speech(self, message):
        with self.lock:  # Lock ile ses dosyasını yazıya çevirme işlemi
            osName = name
            print(f"{osName} işletim sisteminde mesaj sese dönüştürülüyor...")

            if osName != "nt":
                print("Mesaj sese dönüştürülüyor...")
                tts = gTTS(message, lang="tr")
                audio_stream = io.BytesIO()
                tts.write_to_fp(audio_stream)  
                audio_stream.seek(0)  
                pygame.mixer.init()
                with tempfile.NamedTemporaryFile(delete=True, suffix=".mp3") as temp_file:
                    temp_file.write(audio_stream.read())
                    temp_file.flush()
                    pygame.mixer.music.load(temp_file.name)
                    pygame.mixer.music.play()
                    while pygame.mixer.music.get_busy():
                        continue
                print("Mesaj çalındı.")
            
            else:
                tts = gTTS(message, lang="tr")
                audio_stream = io.BytesIO()
                tts.write_to_fp(audio_stream)
                audio_stream.seek(0)
                pygame.mixer.init()

                temp_file_path = "tempTTS.mp3"  # Kalıcı bir dosya yolu
                with open(temp_file_path, "wb") as temp_file:
                    temp_file.write(audio_stream.read())

                try:
                    pygame.mixer.music.load(temp_file_path)
                    pygame.mixer.music.play()
                    while pygame.mixer.music.get_busy():
                        continue
                finally:
                    pygame.mixer.music.unload()  
                    print("Mesaj çalındı.")
                    remove(temp_file_path)

    def runSpeechManager(self):
        while self.runningFlag:
            try:
                print("Ses kaydedilecek.")
                self.record_audio(duration=3, filename= "output.wav")

                print("Kayıt yazıya çevriliyor...")
                text_result = self.audio_to_text("output.wav")

                if text_result is not None:
                    print("Yazıya çevirme başarılı.")

                    if "kapat" in text_result or "kapa" in text_result:
                        self.text_to_speech(f"Sistem kapatılıyor. {text_result} dendi")
                        self.runningFlag = False
                        break

                    print("Yapay zeka devreye giriyor...")
                    modelResult = self.chain.invoke({"context": self.context, "question": text_result})

                    print(f"Modelin yanıtı: {modelResult}")
                    self.text_to_speech(f"{modelResult} dendi")
                    self.context += f"\nUser: {text_result}\nAI: {modelResult}"

                else:
                    print("Yazıya çevirme başarısız oldu.")
                    self.text_to_speech("Tekrar edin")

            except Exception as runSpeechManagerError:
                print(f"Ses motoru yürütme işleminde hata oldu {runSpeechManagerError}")
        
        exit(0)

    def startSpeechManager(self):
        self.t.start()
    
    def checkStopFlag(self):
        return self.runningFlag