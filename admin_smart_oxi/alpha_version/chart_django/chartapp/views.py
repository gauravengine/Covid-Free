from django.shortcuts import render
from .models import state,patient
from django.db.models import Avg  
import json
# Create your views here.
state_dic={
    '1' :  'Haryana',
    '2' : 'Delhi',
    '3' : 'Rajasthan',
    '4' : 'Himachal Pradesh',
    '5' : 'Uttarakhand',
    '6' : 'Punjab',
    '7' : 'Tamil Nadu',
    '8' : 'Karnataka',
}
def index(request):
    querySet=patient.objects.values('state').annotate(avgSpo2=Avg('spo2'))
    querySet2=patient.objects.values('state').annotate(avgbpm=Avg('bpm'))
    print(querySet)
    print(querySet2)
    print(type(querySet))
    my_dic={}
    labels=[]
    data=[]
    bpm_labels=[]
    bpm_data=[]
    for payload in querySet:
        # print(payload['state'])
        state_id=str(payload['state'])
        # print(payload['avgSpo2'])
        my_dic[state_dic[state_id]]=payload['avgSpo2']
        labels.append(state_dic[state_id])
        data.append(payload['avgSpo2'])
        
    for payload in querySet2:
        state_id=str(payload['state'])
        # print(payload['avgSpo2'])
        # my_dic[state_dic[state_id]]=payload['avgSpo2']
        bpm_labels.append(state_dic[state_id])
        bpm_data.append(payload['avgbpm'])

    print("my_dic")
    print(labels)
    print(data)
    print(bpm_labels)
    print(bpm_data)
    # data = "current data"
    context ={
        "labels": labels,
        "data" : data
    }
    return render(request,'chartapp/index.html',{
    'labels': labels,
    'data': data,
    'bpm_labels':bpm_labels,
    'bpm_data':bpm_data,
    })

def calc(request):
    total_oxygen=100
    if(request.method=="POST"):
        total_oxygen=int(request.POST["quantity"])
    
    print(total_oxygen)
    querySet=patient.objects.values('state').annotate(avgSpo2=Avg('spo2'))
    all_entry=patient.objects.values()
    print(all_entry)
    # querySet2=patient.objects.values('state').annotate(avgbpm=Avg('bpm'))
    print("total no of entries :")
    total_people=all_entry.count()

    # print(querySet2)
    print(type(querySet))
    my_dic={}
    labels=[]
    data=[]
    bpm_labels=[]
    bpm_data=[]
    sum_avgspo2=0
    for payload in querySet:
        sum_avgspo2+=100-payload['avgSpo2']
    for payload in querySet:
        # print(payload['state'])
        state_id=str(payload['state'])
        # print(payload['avgSpo2'])
        my_dic[state_dic[state_id]]=payload['avgSpo2']
        this_state_people=patient.objects.filter(state_id=state_id)
        print(this_state_people.count())
        labels.append(state_dic[state_id])
        curr=((100-payload['avgSpo2'])*(this_state_people.count()))/((sum_avgspo2)*(this_state_people.count()) )
        data.append(curr*total_oxygen)
        
    

    print("my_dic")
    print(labels)
    print(data)
    print(bpm_labels)
    print(bpm_data)
    # data = "current data"
    print(data)
    context ={
        "labels": labels,
        "data" : data
    }
    return render(request,'chartapp/calc.html',{
    'labels': labels,
    'data': data,
    
    })