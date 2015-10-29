/* 
* @Author: LiSnB
* @Date:   2015-04-06 14:06:28
* @Last Modified by:   LiSnB
* @Last Modified time: 2015-10-29 20:43:35
*/

$(document).ready(function(){

    var bubbleit = true;
    var ishandled = false;
    function intro(){
        var keywords = ["SMART", "PERSONAL", "INTELLISENSE CAPABLE", "TEXT INPUTTING TECHNOLOGY", "IS COMMING TO YOUR", "WINDOWS", "AND", "WINDOWS PHONE", "INTRODUCING","WORDSENSE."];
        var interval  = 1500;
        var i = 0;
        (function settext(){
            if(i==keywords.length){
                setTimeout($('#commingsoon').animate({opacity:1}, 2000), 1000);
                return ;
            }
            $("#keyword").text(keywords[i]);
            i+=1;
            if(i<=6)
                interval/=(1+0.05*i);
            setTimeout(settext,interval)
        })();

        if(document.body.clientWidth<800){
            ishandled = true;
        }
        var bubblecnt = 0;

        function randomChar(){
            var cate = Math.random();
            if(cate<0.8){
                var word = (Math.round(Math.random() * 20901) + 19968).toString(16);
            }else if(cate<0.9){
                var word = "00"+(Math.round(Math.random()*26)+ 97).toString(16);
            }else{
                var word = "00"+(Math.round(Math.random()*26)+65).toString(16);
            }
            return unescape('%u'+word);
            
        }

        function bubble(){
            console.log(bubbleit)
            if(!bubbleit)
                return;
            if (bubblecnt>100){
                $(".bubble").remove();
                $(".square").remove();
                bubblecnt=0;
            }
            if(bubblecnt>40){
                $(".bubble:lt(30)").remove();
                $(".square:lt(80)").remove();
                bubblecnt-=30;
            }
            
            for (var i = 0; i < Math.random()*4; ++i){
                var left = Math.random()*100;
                var size = Math.random()*2+0.2;
                var scale = size.toString()+"em";
                if(size>1.2)
                    var fontsize = "1em";
                else if(size>0.9)
                    var fontsize = "0.25em";
                else 
                    var fontsize = "0em";
                // var scale = "scale("+(Math.random()*3).toString()+")";
                var tdiv = $("<div></div>").addClass("bubble");
                bubblecnt++;
                tdiv.css(
                    {
                        left:left.toString()+"%",
                        width:scale,
                        height:scale,
                        opacity:Math.random(),
                        fontSize:fontsize
                    });
                if(size>0.9){
                    tdiv.text(randomChar());
                    // if(Math.random()>0.5)
                    //     tdiv.text("环")
                    // else
                    //     tdiv.text("手")
                }
                    
                $("#splash").append(tdiv);
            }
            for (var i = 0; i< Math.random()*4; ++i){
                var left = Math.random()*100;
                var tdiv = $("<div>").addClass("square");
                tdiv.css({
                    left:left.toString()+"%"
                });
                $("#splash").append(tdiv);
            }

            setTimeout(bubble, Math.random()*1000);
        };

        if(!ishandled)
            bubble();
    }

    var videos = [false, false, false, false, false, false, false];

    function showvideo(index){
        if(ishandled || index<1 || index > 7||videos[index-1])
            return ;
        var videopath = "./media/WS"+index.toString()+".mp4";
        // var video = $("#WS"+index.toString());
        var video = document.getElementById("WS"+index.toString())
        video.src = videopath;
        video.onloadeddata=function(){
            setTimeout(function(){
                $(video).animate({opacity:1}, 1000);
                videos[index-1]=true;   
            }, 1000);
        }
        setTimeout(function(){video.load();}, 1000);
        
    }
    function playvideo(index){
        if(index<1 || index > 7)
            return ;
        var video = $("#WS"+index.toString()).get(0);
        setTimeout(function(){
                video.play();
            }, 1000);
    }
    function pausevideo(index){
        if(index<1 || index > 7)
            return ;
        var video = $("#WS"+index.toString()).get(0);
        video.pause();
    }

    $('#fullpage').fullpage({
                anchors: ['intro', 'v1', 'v2','v3','v4', 'v5', 'v6', 'v7', 'windowsphone', 'plus', 'thankyou'],
                sectionsColor: ['#4285F4','white','white','white','white','white','white','white','#333f50','#c84400','#548235','#C63D0F', '#1BBC9B', '#000000'],
                navigation: true,
                navigationPosition: 'right',
                navigationTooltips: ['INTRO', 'TEXT COMPLETION', 'KEEN LEARNER','ANSWERS YOUR QUESTIONS','INTELLISENSE', 'KNOWS YOUR FEELINGS', 'WE DO MATH TOO', 'STYLE YOUR TEXTS', 'WORDSENSE','PLUS', 'THANK YOU'],
                scrollingSpeed: 1000,
                afterLoad:function(anchorlink, index){
                    // console.log(index);
                    //showvideo(index-1);
                },
                afterRender:function(){
                    intro();
                },
                onLeave:function(index, nextIndex, direction){
                    console.log(index, nextIndex);
                    pausevideo(index-1);
                    showvideo(nextIndex-1);
                    playvideo(nextIndex-1);
                    if(index==1)
                        bubbleit = false;
                }
    });
});