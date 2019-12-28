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

class Login(View):
    def get(self, request, *args, **kwargs):
        return render(request,template_name='manage_accounts/login.html',context={})

    def post(self, request, *args, **kwargs):
        pass

class Profile(View):
    def get(self, request, *args, **kwargs):
        return render(request,template_name='manage_accounts/profile.html',context={})

    def post(self, request, *args, **kwargs):
        pass

class Register(View):
    def get(self, request, *args, **kwargs):
        return render(request,template_name='manage_accounts/register.html',context={})
        # return HttpResponse('Register Page created')

    def post(self, request, *args, **kwargs):
        username = request.POST["uname"]
        fname = request.POST["fname"]
        lname = request.POST["lname"]
        email = request.POST["email"]
        phone_num = request.POST["phone_num"]
        raw_password = request.POST["password"]

        hashed_password = make_password(raw_password)
        print(hashed_password)

        with connection.cursor() as cursor:
                sql = "INSERT INTO accounts(username, fname, lname, email, phone_num, password) VALUES('{}','{}','{}','{}','{}','{}')".format(username, fname, lname, email, phone_num, hashed_password)
                cursor.execute(sql)
                connection.commit()
        return redirect("/")