from flask import Flask, render_template, url_for, jsonify, request, json
from flask_mysqldb import MySQL
from flask_cors import CORS

app = Flask(__name__)
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PASSWORD'] = ''
app.config['MYSQL_HOST'] = 'localhost'
app.config['MYSQL_DB'] = 'smart_bag_db'
app.config['MYSQL_CURSORCLASS'] = 'DictCursor'
MySQL = MySQL(app)
CORS(app)

@app.route('/post/notif', methods=['POST','GET'])
def index():
    conn = MySQL.connection
    cursor = conn.cursor()

    if request.method == 'POST':
        print_id = request.json['print_id']
        if print_id != "0":
            cursor.execute('SELECT fname FROM registered_prints WHERE print_id = {}'.format(print_id))
            fname = cursor.fetchone()["fname"]
            print(fname)

            cursor.execute('SELECT lname FROM registered_prints WHERE print_id = {}'.format(print_id))
            lname = cursor.fetchone()["lname"]
            print(lname)

            cursor.execute('INSERT INTO notifs(status, user, accounts_acc_id, registered_prints_print_id) VALUES("Success", "{} {}", 1, {})'.format(fname, lname, print_id))
            conn.commit()
        else:
            cursor.execute('INSERT INTO notifs(status, user, accounts_acc_id) VALUES("Error", "Unknown", 1)')
            conn.commit()
    
    elif request.method == 'GET':
        return "Success GET"

    
    return "POST request success"


if __name__ == "__main__":
    app.run(debug = True, host= '192.168.254.107')