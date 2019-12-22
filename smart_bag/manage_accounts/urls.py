from django.urls import path, include
from . import views

app_name = "manage_accounts"

urlpatterns = [
    path('',views.Login.as_view(),name='login'),
    path('profile/',views.Profile.as_view(), name='profile'),
]