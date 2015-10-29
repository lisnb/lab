/* 
* @Author: LiSnB
* @Date:   2015-04-09 12:34:33
* @Last Modified by:   LiSnB
* @Last Modified time: 2015-04-09 12:40:32
*/

$(document).ready(function(){
    var mediaroot="./media/";
    var videoindex=window.location.hash.substr(1).trim()||"1";
    var videofilename = mediaroot+"WS"+videoindex+".mp4";
    var video = document.getElementById("wsintro");
    video.src = videofilename;
    video.load();
    video.play();
});