#!/bin/python

import dis_client
import commands
import json

search_list = "275836:75:151381462,275847:360:479181129,275658:250:491930764,276775:1090:1902546029,276525:2453:4196742942,276501:141:232625640,276501:1277:2150454271,276811:502:912019671,277069:189:237076988,277305:372:562512901,278770:390:752183616,278509:106:62909241,278345:314:469092385,278509:623:966895086,279766:671:1247479543,279115:298:424149717"
list_of_events = search_list.split(",")

dataset_prefixes = ["JetHT"]

for prefix in dataset_prefixes:
    data = dis_client.query(q="/%s/Run2016*-23Sep2016*/MINIAOD" % (prefix))
    list_of_datasets = data['response']['payload']
    for dataset in list_of_datasets:
        print dataset
        for event in list_of_events:
            cms3_data = dis_client.query(q="%s,%s" % (dataset, event), typ="pick_cms3")
            if not cms3_data['response']['fail_reason'] == '':
                continue
            if not cms3_data['response']['payload']['files']:
                continue
            print dataset
            print "\t%s %s" % (cms3_data['response']['payload']['files'], event.replace(":", ", "))
            if cms3_data['response']['payload']['skim_command']:
                print "\t%s" % (cms3_data['response']['payload']['skim_command'])
                status,output=commands.getstatusoutput("./%s -o %s" % (cms3_data['response']['payload']['skim_command'], cms3_data['response']['payload']['files'][0].split("/")[-1]))
