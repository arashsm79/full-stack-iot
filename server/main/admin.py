from django.contrib import admin

# Register your models here.

from .models import Timeseries, Device

admin.site.register(Timeseries)
admin.site.register(Device)
