# Generated by Django 4.0.4 on 2022-05-07 17:12

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Device',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=128, unique=True)),
                ('access_token', models.CharField(max_length=128, unique=True)),
                ('is_gateway', models.BooleanField(default=False)),
            ],
        ),
        migrations.CreateModel(
            name='Timeseries',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('key', models.CharField(max_length=128)),
                ('value', models.CharField(max_length=128)),
                ('timestamp', models.IntegerField()),
                ('device_id', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='main.device')),
            ],
        ),
    ]
