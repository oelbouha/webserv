
import string
import random
import os
import os.path
import csv
import sys

def new_session(dir) -> str:
    chars = string.ascii_letters + string.digits
    key_len = 24
    key = ''.join(random.choices(chars, k=key_len))
    while os.path.isfile(dir + key):
        key = ''.join(random.choices(chars, k=key_len))
    return key

class Session:
    def __init__(self, key, dir) -> None:
        self.dir = dir
        self.key = key
        self.data = {}
        self.file = open(self.dir + self.key, mode='a')
        self.load()

    def load(self) -> None:
        with open(self.dir + self.key, mode='r') as input_file:
            for row in input_file:
                pair = row.rstrip('\n').split(",", 1)
                self.data[pair[0]] = pair[1]
    
    def set(self, key, value) -> None:
        self.data[key] = value

    def get(self, key) -> str:
        return self.data[key]

    def save(self) -> None:
        writer = csv.writer(self.file)
        for key in self.data:
            writer.writerow([key, self.data[key]])
        self.file.flush()
    
    def get_session_header(self) -> str:
        header = "Set-Cookie: session_id=%s; Max-Age=20\r" % self.key
        return header

