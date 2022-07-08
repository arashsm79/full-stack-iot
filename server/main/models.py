from django.db import models

class Device(models.Model):
    name = models.CharField(max_length=128, unique=True)
    is_gateway = models.BooleanField(default=False)

class Timeseries(models.Model):
    device = models.ForeignKey(Device, on_delete=models.CASCADE)
    key  = models.CharField(max_length=128)
    value  = models.CharField(max_length=128)
    timestamp = models.IntegerField()
