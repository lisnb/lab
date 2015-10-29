/* 
* @Author: LiSnB
* @Date:   2015-06-12 20:27:45
* @Last Modified by:   LiSnB
* @Last Modified time: 2015-06-12 21:19:05
*/

$(document).ready(function  () {
    $(document).keydown(function  (event) {
        Hackertyper.coding(event);
    })
});

var Hackertyper = {

    code: null,
    index: 0,
    speed: 5,
    file: "",
    init: function() {
        $.get(Hackertyper.file, function(data){
            Hackertyper.code = data;
        });
    },
    update: function(snippet){
        $("#code").html(snippet);
        window.scrollBy(0,50);
        return false;
    },

    coding: function  (key) {
        console.log(key.keyCode);
        if(Hackertyper.code){
            if(key.preventDefault && key.keyCode != 122 && key.keyCode != 116) {
                key.preventDefault();
            };
            if(key.keyCode != 8){
                Hackertyper.index += Hackertyper.speed;
            }else{
                if(Hackertyper.index>Hackertyper.speed)
                    Hackertyper.index -= Hackertyper.speed;
                else
                    Hackertyper.index = 0;
            }
            var snippet = Hackertyper.code.substring(0,Hackertyper.index);
            var rtn = new RegExp("\n","g");
            var rts = new RegExp("\\s","g");
            var rtt = new RegExp("\\t","g");
            snippet = snippet.replace(rtn,"<br>").replace(rtt,"&nbsp;&nbsp;&nbsp;&nbsp;").replace(rts,"&nbsp;");
            Hackertyper.update(snippet);
        }
    }
}