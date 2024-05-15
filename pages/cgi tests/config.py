
config = {
    "SESSION_DIR": "/Users/ysalmi/code/ws/pages/cgi tests/sessions/"
}

def get(key) -> str:
    return config[key]