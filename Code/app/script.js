function psnr(inputImage, outputImage, width, height) {
    let eqm = 0;
    for (let i = 0; i < width * height; ++i) {
        for (let j = 0; j < 3; ++j) {
            eqm += (inputImage[i * 3 + j] - outputImage[i * 3 + j]) * (inputImage[i * 3 + j] - outputImage[i * 3 + j]);
        }
    }
    eqm /= (3.0 * width * height);
    return 10.0 * Math.log10(65025.0 / eqm);
}

$(() => {

    let segmentationMode = 0;
    let imageData;
    
    let canvas = $("#outputImage")[0];
    let context = canvas.getContext("2d");

    $("#inputImage").on("click", function() {
        $("#inputUpload").trigger("click");
    });

    $("#inputUpload").on("change", function() {
        let inputImage = $(this).prop("files")[0];
        if (inputImage) {
            let reader = new FileReader();
            reader.onload = function(event) {
                $("#inputImage").attr("src", event.target.result);
                $("#segmentButton").attr("disabled", false);

                let image = new Image();
                image.onload = function() {
                    canvas.width = image.width;
                    canvas.height = image.height;

                    context.drawImage(image, 0, 0, canvas.width, canvas.height);

                    imageData = context.getImageData(0, 0, image.width, image.height).data;
                }
                image.src = event.target.result;
            }
            reader.readAsDataURL(inputImage);
        }
    });

    $(".segmentationSelect").on("click", function() {
        $(".segmentationSelect").removeClass("btn-success").addClass("btn-primary");
        $(this).addClass("btn-success");
        $("#segmentationParameter1, #segmentationParameter2").val(0);
    })

    $("#segmentationSLIC").on("click", function() {
        segmentationMode = 0;
        $("#labelParameter1").text("Nombre de superpixels");
        $("#labelParameter2").text("Seuil");
    });

    $("#segmentationFH").on("click", function() {
        segmentationMode = 1;
        $("#labelParameter1").text("Seuil de fusion");
        $("#labelParameter2").text("Taille minimale de superpixels");
    });

    $("#segmentationQS").on("click", function() {
        segmentationMode = 2;
        $("#labelParameter1").text("Taille du noyau");
        $("#labelParameter2").text("Variance du noyau");
    });

    $("#segmentButton").on("click", function() {
        const param1 = +$("#segmentationParameter1").val();
        const param2 = +$("#segmentationParameter2").val();
        const pixelCount = canvas.width * canvas.height;
        const componentCount = 3 * pixelCount;

        const inputPtr = Module._malloc(componentCount);
        const inputImageData = new Uint8Array(Module.HEAPU8.buffer, inputPtr, componentCount);

        for (let i = 0; i < pixelCount; ++i) {
            inputImageData[3 * i] = imageData[4 * i];
            inputImageData[3 * i + 1] = imageData[4 * i + 1];
            inputImageData[3 * i + 2] = imageData[4 * i + 2];
        }

        const outputPtr = Module._malloc(componentCount);
        const outputImageData = new Uint8Array(Module.HEAPU8.buffer, outputPtr, componentCount);

        switch (segmentationMode) {
            case 0 : // SLIC
                Module._slic(inputPtr, outputPtr, canvas.height, canvas.width, param1, param2)
                break;
            case 1 : // Felzenszwalb
                Module._felzenszwalb(inputPtr, outputPtr, canvas.width, canvas.height, param1, param2);
                break;
            case 2 : // Quick Shift :
                Module._quickShift(inputPtr, outputPtr, canvas.width, canvas.height, param1, param2);
                break;
        }

        const segmentedImage = context.createImageData(canvas.width, canvas.height);
        for (let i = 0; i < pixelCount; ++i) {
            segmentedImage.data[4 * i] = outputImageData[3 * i];
            segmentedImage.data[4 * i + 1] = outputImageData[3 * i + 1];
            segmentedImage.data[4 * i + 2] = outputImageData[3 * i + 2];
            segmentedImage.data[4 * i + 3] = imageData[4 * i + 3];
        }

        context.putImageData(segmentedImage, 0, 0);

        $("#psnrIndicator").text(psnr(inputImageData, outputImageData, canvas.width, canvas.height).toFixed(3));

        Module._free(inputPtr);
        Module._free(outputPtr);
    });


});