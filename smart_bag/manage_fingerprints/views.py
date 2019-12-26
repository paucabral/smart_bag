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
        acc_id = 1
        with connection.cursor() as cursor:
            cursor.execute("SELECT * FROM registered_prints WHERE accounts_acc_id={}".format(acc_id))
            registered_prints=dictfetchall(cursor)
            print(registered_prints)

        return render(request,template_name='manage_fingerprints/manage.html',context={'registered_prints':registered_prints})

    def post(self, request, *args, **kwargs):
        pass

class Register(View):
    def get(self, request, *args, **kwargs):
        return render(request,template_name='manage_fingerprints/register.html',context={})

    def post(self, request, *args, **kwargs):
        acc_id = 1
        print_id = request.POST["print_id"]
        fname = request.POST["fname"]
        lname = request.POST["lname"]
        phone_num = request.POST["phone_num"]
        with connection.cursor() as cursor:
            cursor.execute("INSERT INTO registered_prints VALUES({},'{}','{}','{}',{})".format(print_id, fname, lname, phone_num, acc_id))
        return redirect("/fingerprints/manage/")

        