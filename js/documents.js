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

  axios({
    url: filePath,  // Use the file path as the URL for download
    method: 'GET',
    responseType: 'blob'  // Ensures that the response is treated as a blob (binary data)
  })
  .then((response) => {
    if (response.status === 200) {
      const url = window.URL.createObjectURL(new Blob([response.data]));
      const link = document.createElement('a');
      link.href = url;
      link.setAttribute('download', filename); // Use the extracted filename as the download filename
      document.body.appendChild(link);
      link.click();
      document.body.removeChild(link);
      window.URL.revokeObjectURL(url);
    } else {
      console.error('Error: Received non-200 status code');
    }
  })
  .catch((error) => {
    console.error('Error downloading the image:', error);
  });
}