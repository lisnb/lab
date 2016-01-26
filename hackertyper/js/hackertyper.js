/*
* @Author: LiSnB
* @Date:   2015-06-12 09:39:04
* @Last Modified by:   LiSnB
* @Last Modified time: 2016-01-26 20:10:18
*/

$(document).ready(function(){
    $(document).keydown(function(event){
        Hackertyper.coding(event);
    })
});


var Hackertyper = {

    code: null,
    index: 0,
    speed: 5,
    file: "",
    init: function(){
        $.get(Hackertyper.file, function(data){
            Hackertyper.code = data;
        });
    },
    cursor:'<span class="cursor"></span>',
    updatecode: function(snippet){
        snippet+=this.cursor;
        $("#code").html(snippet);
        var code = document.getElementById('code');
        console.log(code.scrollHeight);
        // code.scrollTop = code.scrollHeight;
        // window.scrollBy(0, 50);
        // var scrollHeight = $("#code").scrollTop();//document.getElementById("code").scrollHeight
        // console.log(scrollHeight);
        $("body").scrollTop(code.scrollHeight);
        // return false;
    },

    getcode: function(){
        return $("#code").html();
    },

    coding: function(key){
        // console.log(key.keyCode);
        if(Hackertyper.code){
            if (key.preventDefault && key.keyCode != 122 &&key.keyCode != 116) { // prevent F11(fullscreen) from being blocked
                key.preventDefault();
            };
            // var cont = Hackertyper.getcode();
            if(key.keyCode!=8){
                Hackertyper.index+=Hackertyper.speed;
            }else{
                if(Hackertyper.index>Hackertyper.speed)
                    Hackertyper.index-=Hackertyper.speed;
                else
                    Hackertyper.index=0;
            }
            var snippet = Hackertyper.code.substring(0, Hackertyper.index);
            var rtn = new RegExp("\n", "g"); // newline regex
            var rts = new RegExp("\\s", "g"); // whitespace regex
            var rtt = new RegExp("\\t", "g"); // tab regex
            snippet = snippet.replace(rtn, "<br/>").replace(rtt, "&nbsp;&nbsp;&nbsp;&nbsp;").replace(rts, "&nbsp;");
            Hackertyper.updatecode(snippet);
        }
    }
}