from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('telemetry', views.telemetry, name='telemetry'),
]
