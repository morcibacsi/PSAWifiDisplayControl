#pragma once

#ifndef _index_html_h
    #define _index_html_h

// This file contains the main html page

const char index_html[] PROGMEM = R"=====(
<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <title>PSA WiFi display controller</title>
    <style>
        body {
             background-color: white;
             font-family: -apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,"Noto Sans",sans-serif,"Apple Color Emoji","Segoe UI Emoji","Segoe UI Symbol","Noto Color Emoji";
        }
        table {
             border-collapse: collapse;
             width: 100%;
        }
        th, td {
             border: 0px solid black;
             padding: 8px;
        }
        .aligncenter{
             text-align: center;
        }
        .btn-primary:hover {
             color: #fff;
             background-color: #0069d9;
             border-color: #0062cc;
        }
        .btn-block {
             display: block;
             width: 100%;
        }
        .btn-group-lg > .btn, .btn-lg {
             padding: .5rem 1rem;
             font-size: 1.25rem;
             line-height: 1.5;
             border-radius: .3rem;
        }
        .btn-arrow {
             font-size: 28px !important;
        }
        .btn {
             cursor: pointer;
             display: inline-block;
             font-weight: 400;
             color: #212529;
             text-align: center;
             vertical-align: middle;
             -webkit-user-select: none;
             -moz-user-select: none;
             -ms-user-select: none;
             user-select: none;
             background-color: transparent;
             border: 1px solid transparent;
             border-top-color: transparent;
             border-right-color: transparent;
             border-bottom-color: transparent;
             border-left-color: transparent;
             padding: .375rem .75rem;
             font-size: 1rem;
             line-height: 2.5;
             border-radius: .25rem;
             transition: color .15s ease-in-out,background-color .15s ease-in-out,border-color .15s ease-in-out,box-shadow .15s ease-in-out;
        }
        .btn-primary {
             color: #fff;
             background-color: #007bff;
             border-color: #007bff;
        }

        .h3, h3 {
             font-size: 1.75rem;
             text-align: center !important;
        }
        .h1, .h2, .h3, .h4, .h5, .h6, h1, h2, h3, h4, h5, h6 {
             margin-bottom: .5rem;
             font-weight: 500;
             line-height: 1.2;
        }
         h1, h2, h3, h4, h5, h6 {
             margin-top: 0;
             margin-bottom: .5rem;
        }
    </style>
  </head>
  <body>
    <div style="overflow-x:auto;">
    <table id="table1">
        <tr>
            <td colspan="3"><h3>PSA WiFi display controller</h3></td>
        </tr>
        <tr>
            <td></td>
            <td class="aligncenter" >
                <button type="button" class="btn btn-primary btn-lg btn-block btn-arrow" data-mode="119">&#9650;</button>
            </td>
            <td></td>
        </tr>
        <tr>
            <td>
                <button type="button" class="btn btn-primary btn-lg btn-block btn-arrow" data-mode="97">&#9664;</button>
            </td>
            <td class="aligncenter" >
                <button type="button" class="btn btn-primary btn-lg btn-block btn-arrow" data-mode="101">OK</button>
            </td>
            <td>
                <button type="button" class="btn btn-primary btn-lg btn-block btn-arrow" data-mode="100">&#9654;</button>
            </td>
        </tr>
        <tr>
            <td></td>
            <td class="aligncenter">
                <button type="button" class="btn btn-primary btn-lg btn-block btn-arrow" data-mode="115">&#9660;</button>
            </td>
            <td></td>
        </tr>
        <tr>
            <td>
                <button type="button" class="btn btn-primary btn-lg btn-block" data-mode="110">Mode</button>
            </td>
            <td>
                <button type="button" class="btn btn-primary btn-lg btn-block" data-mode="109">Menu</button>
            </td>
            <td>
                <button type="button" class="btn btn-primary btn-lg btn-block" data-mode="113">&nbsp;Esc&nbsp;&nbsp;</button>
            </td>
        </tr>
    </table>
    </div>

    <!-- Optional JavaScript -->
    <script type="text/javascript">
        const buttons =  document.getElementsByClassName("btn");
        var i;
        for (i = 0; i < buttons.length; i++) {
            buttons[i].addEventListener("click", function(event) {
                event.preventDefault();

                var request = new XMLHttpRequest();
                var url = "/rest/button";
                request.open("POST", url, true);
                request.setRequestHeader("Accept", "application/json, text/javascript, */*; q=0.01");
                request.setRequestHeader("Content-Type", "application/json");
                request.setRequestHeader("X-Requested-With", "XMLHttpRequest");
                request.onreadystatechange = function () {
                    if (request.readyState === 4 && request.status === 200) {
                        var jsonData = JSON.parse(request.response);
                        console.log(jsonData);
                    }
                };

                var data = {"button": parseInt(this.dataset.mode)};
                var jsonData = JSON.stringify(data);
                request.send(jsonData);
            });
        }
    </script>

  </body>
</html>
)=====";
#endif