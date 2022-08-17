from django.db import models

# Create your models here.
class state(models.Model):
    name = models.CharField(max_length=300)

class patient(models.Model):
    name = models.CharField(max_length=300)
    spo2 = models.IntegerField()
    # bpm = models.DecimalField(max_digits=10, decimal_places=2)
    bpm = models.IntegerField()
    state = models.ForeignKey(state, on_delete=models.CASCADE)
    