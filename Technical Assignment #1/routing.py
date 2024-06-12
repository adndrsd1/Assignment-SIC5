from flask import Flask, request, render_template

app = Flask(__name__, template_folder='templates')

dict_data = {
    "name" : [],
    "sensorValue" : [],
    "voltage" : []
}
@app.route('/', methods=['GET'])
def get_data():
    return dict_data

@app.route('/data', methods=['POST'])
def post_data():
    request.method == "POST"
    data = request.get_json()
    dict_data["name"].append(data["name"])
    dict_data["sensorValue"].append(data["sensorValue"])
    dict_data["voltage"].append(data["voltage"])
    return f"{data['name']} berhasil mengirimkan data {data['value']}", 201

if __name__ == '__main__':
    app.run(debug=True, host="172.23.13.247", port=8000)