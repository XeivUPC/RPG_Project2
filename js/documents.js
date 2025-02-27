var slideIndex = [1,1];
var slideId = ["slideshow1"]

function plusSlides(n, no) {
  showSlides(slideIndex[no] += n, no);
}

function showSlides(n, no) {
  var i;
  var x = document.getElementsByClassName(slideId[no]);
  console.log(x);
  if (n > x.length) {slideIndex[no] = 1}    
  if (n < 1) {slideIndex[no] = x.length}
  for (i = 0; i < x.length; i++) {
     x[i].style.display = "none";  
  }
  x[slideIndex[no]-1].style.display = "block";  
  filePath = x[slideIndex[no]-1].getAttribute('alt');
}

var filePath = "";
function downloadFile(){

  plusSlides(0,0);
  var filename = filePath.replace(/^.*[\\/]/, '');

  fetch('https://jsonplaceholder.typicode.com/todos/1')
  .then(resp => resp.status === 200 ? resp.blob() : Promise.reject('something went wrong'))
  .then(blob => {
    const url = window.URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.style.display = 'none';
    a.href = url;
    a.download = filename;
    document.body.appendChild(a);
    a.click();
    window.URL.revokeObjectURL(url);
    // or you know, something with better UX...
    alert('File Downloaded'); 
  })
  .catch(() => alert('oh no!'));
}