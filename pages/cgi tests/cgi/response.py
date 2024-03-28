

class Response:
    def __init__(self) -> None:
        self.headers = {}
        self.body = ""

    def setHeader(self, key, val):
        self.headers[key] = val

    def setBody(self, body):
        self.body = body

    def send(self):
        for header in self.headers:
            print(header, ": ", self.headers[header], "\r")
        print("\r")
        print(self.body)