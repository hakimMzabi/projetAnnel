import sys

import os
from io import BytesIO
from django.shortcuts import render
from django.http import JsonResponse
from decoder.decoder import launch_traitment
import application


def home(request):

    dirname = os.path.dirname(__file__)
    dirpath = os.path.join(dirname, "../../Models/")

    filelist = os.listdir(dirpath)

    files = []
    for filename in filelist:
        if filename.split(".")[-1].lower() == 'model':
            files.append(filename)

    return render(request, 'index.html', locals())


def upload(request):
    if request.is_ajax():
        image = request.FILES.get("image")
        model_name = request.POST.get("modelName")

        type_is_correct = image.content_type in application.settings.VALID_TYPES

        if image and type_is_correct:
            response = launch_traitment(image, model_name)
            print(response)
            print()
            print()
            print("----")
            print()
            print()
            return JsonResponse(response)

        else:
            return JsonResponse({"error": "Erreur ! Type d'image non supportée !"}, status=500)
    else:
        return JsonResponse({"error": "Erreur ! not xmlhttprequest"}, status=500)



