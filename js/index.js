function setHTML(htmlAddres)
{
    const frame = document.getElementById("frame");
    if(frame.getAttribute("src") == "./html/"+ htmlAddres +".html"){
        return;
    }
    frame.setAttribute("src", "./html/"+ htmlAddres +".html");
}