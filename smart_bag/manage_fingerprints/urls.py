from django.urls import path, include
from . import views

app_name = "manage-fingerprints"

urlpatterns = [
    path('manage/',views.Manage.as_view(),name='manage_prints'),
    path('register/',views.Register.as_view(),name='register_prints'),
]