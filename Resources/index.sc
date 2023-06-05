let title = "window";
let html = "
<html>
    <head> 
        <style>
            body
            { 
                background:url(\"C:\\backgroud.jpg\") no-repeat fixed center;
                background-size:cover; 
            }
        </style>
    </head>
    <body>
        Accout: <input type=\"text\"  id=\"ac\"></br>
        Passwd: <input type=\"password\"  id=\"pwd\"></br>
        <button onclick=\"myFunction()\">SubMit</button>

        <script>
            function myFunction() {
                var accout = document.getElementById(\"ac\").value;
                var password = document.getElementById(\"pwd\").value;
                alert(\"Accout: \"+accout +\"\\nPassword: \"+ password);
            }
        </script>
    </body>
</html>
";