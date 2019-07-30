import os
i = 0    
filepath = input("Dossier à renommer : ")
filepath = filepath.rstrip(' ') + '/'.replace("\\ ", ' ')

validExt = {'jpg', 'png', 'gif', 'jpeg'}

for filename in os.listdir(filepath):
    if filename.split(".")[-1].lower() in validExt:
        src = filepath + filename 
        dst =filepath + "img-" + str(i) + ".jpg" 
        os.rename(src, dst) 
        i += 1