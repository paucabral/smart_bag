from django.shortcuts import render, redirect
from django.http import HttpResponse
from django.views import View
from django.db import connection
from django.contrib.auth.hashers import make_password, check_password
from operator import itemgetter

# Create your views here.
def dictfetchall(cursor): 
    "Returns all rows from a cursor as a dict" 
    desc = cursor.description 
    return [
            dict(zip([col[0] for col in desc], row)) 
            for row in cursor.fetchall() 
    ]

class Manage(View):
    def get(self, request, *args, **kwargs):
        return HttpResponse("Manage prints Working!")

    def post(self, request, *args, **kwargs):
        pass

class Register(View):
    def get(self, request, *args, **kwargs):
        return HttpResponse("Register prints Working!")

    def post(self, request, *args, **kwargs):
        pass