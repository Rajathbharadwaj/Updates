import requests

url = "https://nlp-translation.p.rapidapi.com/v1/translate"

querystring = {"text":"Hello, world!!","to":"es","from":"en"}

headers = {
    'x-rapidapi-host': "nlp-translation.p.rapidapi.com",
    'x-rapidapi-key': "bc74f50573msh87b1b9e5be65695p10ede2jsn0ed016c147ce"
    }

response = requests.request("GET", url, headers=headers, params=querystring)

print(response.text)
