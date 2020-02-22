from flask import Flask, render_template, url_for, jsonify, request, json
from flask_mysqldb import MySQL

app = Flask(__name__)
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PASSWORD'] = ''
app.config['MYSQL_HOST'] = 'localhost'
app.config['MYSQL_DB'] = 'smart_bag_db'
app.config['MYSQL_CURSORCLASS'] = 'DictCursor'
MySQL = MySQL(app)

@app.route('/post/notif', methods=['POST'])
def index():
    conn = MySQL.connection
    cursor = conn.cursor()
    
    print_id = request.json['print_id']

    if print_id != "-1":
        cursor.execute('SELECT fname FROM registered_prints WHERE print_id = {}'.format(print_id))
        fname = cursor.fetchone()["fname"]
        print(fname)

        cursor.execute('SELECT lname FROM registered_prints WHERE print_id = {}'.format(print_id))
        lname = cursor.fetchone()["lname"]
        print(lname)

        cursor.execute('INSERT INTO notifs(status, user, accounts_acc_id, registered_prints_print_id) VALUES("Success", "{} {}", 1, {})'.format(fname, lname, print_id))
        conn.commit()
    #cursor.execute("UPDATE alumni SET ispaid = 1 WHERE fname = '{}' AND lname = '{}' AND email = '{}' AND isregistered = 1".format(fname, lname, email))
    #conn.commit()
    
    return "return render_template() here"


if __name__ == "__main__":
    app.run(debug = True)