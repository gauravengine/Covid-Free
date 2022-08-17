# Generated by Django 4.0.3 on 2022-03-21 08:21

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='state',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=300)),
            ],
        ),
        migrations.CreateModel(
            name='patient',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=300)),
                ('spo2', models.IntegerField()),
                ('bpm', models.DecimalField(decimal_places=2, max_digits=10)),
                ('state', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='chartapp.state')),
            ],
        ),
    ]
