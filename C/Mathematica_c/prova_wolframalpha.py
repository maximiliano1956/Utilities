#!/usr/bin/env python3
"""
Wolfram API implementation for Python.
Returns queries as a text file.
Carlos Saucedo, 2019
"""
import wolframalpha

class Client(object):
    def __init__(self, key):
        self.key = key
        self.client = wolframalpha.Client(key)
    
    # Processes a user query.
    def ask(self, queryText):
        res = self.client.query(queryText)
        return next(res.results).text


aaa=Client("5V39KT-9968V43Q82")
res=Client.ask(aaa,"resultant(x^17+9,(x+1)^17+9)")
print(res)
com=res+"is prime ?"
ttt=Client.ask(aaa,com)
print(ttt)
com="solve{x^17+9="+res+"*y && (x+1)^17+9="+res+"*z)} over integers"
ttt=Client.ask(aaa,com)
print(ttt)
