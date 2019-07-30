import sys

from django.urls import path

from decoder import views

urlpatterns = [
    path('', views.home),
    path('upload', views.upload)
]
