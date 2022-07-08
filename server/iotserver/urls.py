from django.contrib import admin
from django.urls import path, include
import main

urlpatterns = [
    path("main/", include("main.urls")),
    path("telemetry", main.views.telemetry),
    path("admin/", admin.site.urls),
]
