from django.urls import path, include
from . import views

app_name = "notifs"

urlpatterns = [
    path('',views.Home.as_view(),name='home'),
    path('notifs/',views.Notifs.as_view(),name='notifs'),
    path('logs/',views.Logs.as_view(),name='logs'),
]