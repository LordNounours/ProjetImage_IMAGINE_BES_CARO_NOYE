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

function ssim(inputImage, outputImage, width, height) {
    let k1 = 0.01;
    let k2 = 0.03;
    let L = 255;
    let c1 = (k1 * L) * (k1 * L);
    let c2 = (k2 * L) * (k2 * L);
    let c3 = c2 / 2;
    let ssim = [0, 0, 0];
    let count = 0;

    // Fenêtre de 8 par 8
    for (let i = 0; i < height - 8; i += 2) {
        for (let j = 0; j < width - 8; j += 2) {
            count++;
            let pos = i * width + j;

            let moy1 = [0, 0, 0];
            let moy2 = [0, 0, 0];
            let var1 = [0, 0, 0];
            let var2 = [0, 0, 0];
            let cov = [0, 0, 0];
            for (let k = 0; k < 8; k++) {
                for (let l = 0; l < 8; l++) {
                    for (let c = 0; c < 3; c++) {
                        moy1[c] += inputImage[3 * (pos + k + l * width) + c];
                        moy2[c] += outputImage[3 * (pos + k + l * width) + c];
                    }
                }
            }

            for (let c = 0; c < 3; c++) {
                moy1[c] /= 64;
                moy2[c] /= 64;
            }
            for (let k = 0; k < 8; k++) {
                for (let l = 0; l < 8; l++) {
                    for (let c = 0; c < 3; c++) {
                        var1[c] += Math.pow(inputImage[3 * (pos + k + l * width) + c] - moy1[c], 2);
                        var2[c] += Math.pow(outputImage[3 * (pos + k + l * width) + c] - moy2[c], 2);
                        cov[c] += (inputImage[3 * (pos + k + l * width) + c] - moy1[c]) * (outputImage[3 * (pos + k + l * width) + c] - moy2[c]);
                    }
                }
            }

            for (let c = 0; c < 3; c++) {
                var1[c] /= 64;
                var2[c] /= 64;
                cov[c] /= 64;
            }
            for (let c = 0; c < 3; c++) {
                let num = (2 * moy1[c] * moy2[c] + c1) * (2 * Math.sqrt(var1[c]) * Math.sqrt(var2[c]) + c2) * (cov[c] + c3);
                let den = (Math.pow(moy1[c], 2) + Math.pow(moy2[c], 2) + c1) * (var1[c] + var2[c] + c2) * (Math.sqrt(var1[c]) * Math.sqrt(var2[c]) + c3);
                ssim[c] += num / den;
            }
        }
    }

    for (let c = 0; c < 3; c++) {
        ssim[c] /= count;
    }
    return (ssim[0] + ssim[1] + ssim[2]) / 3;
}

function putCanvas(canvas, image, alphaImage, width, height) {
    const context = canvas.getContext("2d");
    const canvasImage = context.createImageData(width, height);
    for (let i = 0; i < width * height; ++i) {
        canvasImage.data[4 * i] = image[3 * i];
        canvasImage.data[4 * i + 1] = image[3 * i + 1];
        canvasImage.data[4 * i + 2] = image[3 * i + 2];
        if (alphaImage !== undefined) {
            canvasImage.data[4 * i + 3] = alphaImage[4 * i + 3];
        }
        else {
            canvasImage.data[4 * i + 3] = 255;
        }
    }

    canvas.width = width;
    canvas.height = height;

    context.putImageData(canvasImage, 0, 0);
}

function putSize(sizeElement, unitElement, compElement, size, defaultSize) {
    const unit = ["o", "Ko", "Mo", "Go", "To"];
    let i = 0;
    let scaledSize = size / 8;
    while (scaledSize > 1000 && i <= 4) {
        scaledSize /= 1000;
        ++i;
    }
    sizeElement.text(scaledSize.toFixed(2));
    unitElement.text(unit[i]);

    if (compElement !== null) {
        const compRate = defaultSize / size;
        compElement
            .css("color", "hsl(" + Math.max(0, Math.min(100.0, 50.0 * Math.sqrt(Math.max(1.0, compRate) - 1.0))) + ", 100%, 50%)")
            .text(compRate.toFixed(3));
    }
}

$(() => {

    let segmentationMode = 0;
    let imageData;
    let filteredImageData;
    let filteredSegmentedData;

    let width = 0;
    let height = 0;

    let hasEdges = false;

    $("#inputImage, #inputNoneBox").on("click", function() {
        $("#inputUpload").trigger("click");
    });

    $("#inputUpload").on("change", function() {
        $(".imageNone").remove();
        $("#inputImage, #outputImage").attr("hidden", false);
        const inputImage = $(this).prop("files")[0];
        if (inputImage) {
            const reader = new FileReader();
            reader.onload = function(event) {
                $("#edgeButton").attr("disabled", true);
                $("#segmentButton").attr("disabled", false);
                $("#postSegmentation").attr("hidden", true);
                hasEdges = false;

                const inputCanvas = $("#inputImage")[0];
                const inputContext = inputCanvas.getContext("2d");

                const outputCanvas = $("#outputImage")[0];
                const outputContext = outputCanvas.getContext("2d");

                const image = new Image();
                image.onload = function() {
                    width = image.width;
                    height = image.height;

                    inputCanvas.width = width;
                    inputCanvas.height = height;
                    outputCanvas.width = width;
                    outputCanvas.height = height;

                    inputContext.drawImage(image, 0, 0, outputCanvas.width, outputCanvas.height);
                    outputContext.drawImage(image, 0, 0, outputCanvas.width, outputCanvas.height);

                    imageData = outputContext.getImageData(0, 0, width, height).data;
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
        $("#labelParameter2").text("Variance du noyau (x1000)");
    });

    $("#segmentButton").on("click", function() {
        const param1 = +$("#segmentationParameter1").val();
        const param2 = +$("#segmentationParameter2").val();
        const pixelCount = width * height;
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
                Module._slic(inputPtr, outputPtr, height, width, param1, param2)
                break;
            case 1 : // Felzenszwalb
                Module._felzenszwalb(inputPtr, outputPtr, width, height, param1, param2);
                break;
            case 2 : // Quick Shift :
                Module._quickShift(inputPtr, outputPtr, width, height, param2 / 1000.0, param1);
                break;
        }

        filteredImageData = inputImageData.slice();
        filteredSegmentedData = outputImageData.slice();

        putCanvas($("#inputImage")[0], inputImageData, imageData, width, height);

        putCanvas($("#outputImage")[0], outputImageData, imageData, width, height);
        putSize($("#defaultSizeIndicator"), $("#defaultSizeUnit"), null, 24 * width * height);
        $("#postSegmentation").attr("hidden", false);

        const psnrResult = psnr(inputImageData, outputImageData, width, height);
        const ssimResult = ssim(inputImageData, outputImageData, width, height);
        $("#psnrIndicator")
            .css("color", "hsl(" + (10 * (Math.min(40, Math.max(10, psnrResult)) - 10) / 3) + ", 100%, 50%)")
            .text(psnrResult.toFixed(3));
        $("#ssimIndicator")
            .css("color", "hsl(" + ((Math.max(0.5, ssimResult) - 0.5) * 200) + ", 100%, 50%)")
            .text(ssimResult.toFixed(3));

        const intermediatePtr = Module._malloc(componentCount);
        const intermediateImageData = new Uint8Array(Module.HEAPU8.buffer, intermediatePtr, componentCount);

        const defaultSize = 24 * width * height;
        let size = Module._compressionPalette(inputPtr, outputPtr, intermediatePtr, height, width);
        putCanvas($("#paletteCanvas")[0], intermediateImageData, imageData, width, height);
        putSize($("#paletteSizeIndicator"), $("#paletteSizeUnit"), $("#paletteCompIndicator"), size, defaultSize);

        size = Module._compressionPredictif(outputPtr, intermediatePtr, width, height);
        putCanvas($("#predictiveCanvas")[0], intermediateImageData, imageData, width, height);
        putSize($("#predictiveSizeIndicator"), $("#predictiveSizeUnit"), $("#predictiveCompIndicator"), size, defaultSize);

        size = Module._compressionRegion(outputPtr, intermediatePtr, width, height);
        putCanvas($("#regionCanvas")[0], intermediateImageData, imageData, width, height);
        putSize($("#regionSizeIndicator"), $("#regionSizeUnit"), $("#regionCompIndicator"), size, defaultSize);

        Module._free(intermediatePtr);
        Module._free(inputPtr);
        Module._free(outputPtr);

        $("#edgeButton").attr("disabled", false);
        hasEdges = false;
    });

    $("#edgeButton").on("click", function() {
        if (hasEdges) {
            putCanvas($("#inputImage")[0], filteredImageData, imageData, width, height);
            putCanvas($("#outputImage")[0], filteredSegmentedData, imageData, width, height);
        }
        else {
            const pixelCount = width * height;
            const componentCount = 3 * pixelCount;

            const inputPtr = Module._malloc(componentCount);
            const inputImageData = new Uint8Array(Module.HEAPU8.buffer, inputPtr, componentCount);

            const segmentedPtr = Module._malloc(componentCount);
            const segmentedImageData = new Uint8Array(Module.HEAPU8.buffer, segmentedPtr, componentCount);
            
            for (let i = 0; i < componentCount; ++i) {
                inputImageData[i] = filteredImageData[i];
                segmentedImageData[i] = filteredSegmentedData[i];
            }

            const outputPtr = Module._malloc(componentCount);
            const outputImageData = new Uint8Array(Module.HEAPU8.buffer, outputPtr, componentCount);

            Module._displayEdges(inputPtr, segmentedPtr, outputPtr, width, height);
            putCanvas($("#inputImage")[0], outputImageData, imageData, width, height);

            Module._displayEdges(segmentedPtr, segmentedPtr, outputPtr, width, height);
            putCanvas($("#outputImage")[0], outputImageData, imageData, width, height);

            Module._free(outputPtr);
            Module._free(segmentedPtr);
            Module._free(inputPtr);
        }
        hasEdges = !hasEdges;
    });
});