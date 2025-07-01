document.addEventListener("DOMContentLoaded", function () {
    const gameImage = document.getElementById("game-download-image");

    gameImage.addEventListener("click", function () {
        const filePath = "../Assets/EssetAxis-Records_Of_Lies.zip";
        const filename = filePath.replace(/^.*[\\/]/, '');

        axios({
            url: filePath,
            method: 'GET',
            responseType: 'blob'
        })
        .then((response) => {
            if (response.status === 200) {
                const url = window.URL.createObjectURL(new Blob([response.data]));
                const link = document.createElement('a');
                link.href = url;
                link.setAttribute('download', filename);
                document.body.appendChild(link);
                link.click();
                document.body.removeChild(link);
                window.URL.revokeObjectURL(url);
            } else {
                console.error('Error: Received non-200 status code');
            }
        })
        .catch((error) => {
            console.error('Error downloading the game:', error);
        });
    });
});
