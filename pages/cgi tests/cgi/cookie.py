from http.cookies import SimpleCookie


def parse_cookie(raw_cookie) -> dict:
    cookie = SimpleCookie()
    cookie.load(raw_cookie)
    cookies = {k: v.value for k, v in cookie.items()}
    return cookies
