function processResult(data) {
    data = JSON.parse(data);
    if (data['status'] > 0) {
        $('#result').html(createSuccessAlert(data['message']));
    } else {
        $('#result').html(createAlert(data['message']));
    }

    setTimeout(removeResult, 2000);
}

function createSuccessAlert(text) {
    return $('<div data-alert class="alert-box success radius">'
        +  text + '<a href="#" class="close">&times;</a> </div>');
}

function createAlert(text) {
    return $('<div data-alert class="alert-box alert radius">'
        +  text + '<a href="#" class="close">&times;</a> </div>');
}

$('body').on('click', '.close', function() {
    removeResult();
    return false;
});

function removeResult() {
    $('.close').parent().fadeOut(300, function() {
        $(this).remove();
    });
}
