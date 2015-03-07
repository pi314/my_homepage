var ST_LIST = 'LIST';
var ST_ARTICLE = 'ARTICLE';

var StateMachine = { };
var last_readed = 0;

StateMachine.state = '';

StateMachine.state_init = function () {
    StateMachine.state = ST_LIST;
};

StateMachine.enter_article = function (index) {
    console.log('enter article ' + index);
    if ( !articles_ready[index] ) {
        console.log('skip article ' + index);
        return;
    }
    StateMachine.state = ST_ARTICLE;
    $('#articles-list-panel').addClass('hidden');
    $('#article-content-panel').removeClass('hidden');
    $('#article-content'+ index).removeClass('hidden');
    switch (articles_readed[index]) {
    case 'B':
        $('#article-line'+ index +' > .like').text('b');
        break
    case 'M':
        $('#article-line'+ index +' > .like').text('m');
        break
    case '+':
        $('#article-line'+ index +' > .like').text(' ');
        break
    }
    articles_readed[index] = true;
    $('#article-line'+ last_readed +' > .title').removeClass('fG');
    $('#article-line'+ last_readed +' > .re').removeClass('fG');
    last_readed = index;
    $('#article-line'+ last_readed +' > .title').addClass('fG');
    $('#article-line'+ last_readed +' > .re').addClass('fG');

};

StateMachine.leave_article = function () {
    StateMachine.state = ST_LIST;
    $('#articles-list-panel').removeClass('hidden');
    $('#article-content-panel').addClass('hidden');
    $('#article-content-panel > div').addClass('hidden');
    console.log('leave article');
};

