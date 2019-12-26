<h1>Smart Bag</h1>

<h3>About</h3>
<p>This system is created as a compliance project for the course: Embedded Systems under the Computer Engineering Department of the Technological Institute of the Philippines - Quezon City. Its core functionallity deals with the use of biometrics(fingerprint) to securely unlock a bag, along with the implementation of smart features such as web and text notifications. The web application is written in Python 3 using Django web framework.</p>

<h3>Developers</h3>
<p>This system is developed by the following collaborators:</p>
<ul>
  <li>Jose Paulo C. Cabral</li>
  <li>Reinhold Lucana</li>
  <li>John Edward Sam Matienzo</li>
  <li>Gio Raper</li>
</ul>

<h3>Program Requisites</h3>
<p>To ensure that the system works as intended, the following requisites are required in order to run the program.</p>
<ul>
  <li>Python Runtime Environment (Recommended: Python version 3.X)</li>
  <li>Django 3.0.1 (or latest, for pip)</li>
  <li>MySQLClient (for pip)</li>
  <li>PyMySQL (for pip)</li>
  <li>MySQL or XAMPP</li>
  <li>MySQL Workbench (optional)</li>
  <li>Git</li>
  <li>Web Browser</li>
  <li>Text Editor</li>
</ul>
  
<h3>System Migration</h3>
<p>These are the steps for migrating and running the system on another machine.</p>
<ol>
  <li>Install/pip install all requisites. (All requisities can be installed preferrably on a virtual environment. See "Program Requisities" above for the list of requirements)</br><p>Note: To create a virtual environment, use the command: mkvirtualenv *name_of_the_environment* (without the asterisks). Use the created virtual environment using the command: workon *name_of_the_environment* (without the asterisks), then use the "pip install" command along with the packages mentioned. The wheel file for MySQLClient can be located under the pre-requisites folder of the repository.<p/></li>
  <li>Connect to an instance of SQL Server using MySQL. (XAMPP or the official MySQL client can be used)</li>
  <li>Clone the repository on prefered location using git</li>
  <li>Create the database schema using the smart_bag_db.sql. The file can be executed using MySQL workbench. The database ERD can be viewed using MySQL Workbench by using the smart_bag_db.mwb</li>
  <li>Navigate to the folder location of mange.py file. Open CMD on this folder</li>
  <li>Run the script: "python manage.py migrate" (without quotation marks) to migrate required the Django entities on the database.</li>
  <li>Run the script: "python manage.py runserver" to run the system on localhost.</li>
  <li>On a web browser, enter "localhost:8000" (without quotation marks, assuming the web app runs on the local host) on the address bar to go to the homepage.</li>
</ol>
