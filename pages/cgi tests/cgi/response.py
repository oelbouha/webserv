
import sys

class Response:
    def __init__(self) -> None:
        self.headers = []
        self.body = ""

    def setHeader(self, key, val):
        self.headers.append({'key':key, 'val':val})

    def setBody(self, body):
        self.body = body
        self.setHeader("content-length", len(body))

    def send(self):
        for header in self.headers:
            print(header['key'], ": ", header['val'], "\r")
        print("\r")
        print(self.body)