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
        username = request.POST["username"]
        raw_password = request.POST["password"]

        with connection.cursor() as cursor:
            sql = "SELECT*FROM accounts WHERE username = '{}'".format(username)
            cursor.execute(sql)
            try:
                result = dictfetchall(cursor)[0]
                print(result['password'])
            except:
                print("Account does not exists")
                return redirect("/")    
        
        if check_password(raw_password, result['password']):
            print("yehey")
            return redirect("/home")
        else:
            print("boo!")
            return redirect("/")

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

        with connection.cursor() as cursor:
                sql = 'INSERT INTO accounts(username, fname, lname, email, phone_num, password) VALUES("{}","{}","{}","{}","{}","{}")'.format(username, fname, lname, email, phone_num, hashed_password)
                cursor.execute(sql)
        return redirect("/")