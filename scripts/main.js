var state = 'list';


var WEEK_COLOR = {
    'Mon': 'fG',
    'Tue': 'fY',
    'Wed': 'fN',
    'Thu': 'fP',
    'Fri': 'fC',
    'Sat': 'fW',
    'Sun': 'fR',
};


function Article (fname) {
    // static info
    this.fname = fname;
    this.index = 0;
    this.like = false;
    this.date = '????/??/??';
    this.time = '??:??';
    this.week = '?';
    this.sign = '';
    this.title = this.fname;
    this.push = [];
    this.re = false;
    this.raw_content = '';
    this.content = [];

    // dynamic info
    this.loaded = false;
    this.read = false;

    this.parse = function (raw_content) {
        this.raw_content = raw_content;

        var lines = raw_content.split(/\r?\n/g);
        var in_header = true;

        for (var i = 0; i < lines.length; i++) {
            if (in_header) {
                // still in article header
                if (lines[i] == '') {
                    in_header = false;
                }

                var match = /^:(.*?): *(.*)$/.exec(lines[i]);
                if (match == null) {
                    in_header = false;
                    continue;
                }
                // article_info[ match[1] ] = match[2];
                switch (match[1]) {
                    case 'like':
                        this.like = (match[2] === 'true');
                        break;
                    default:
                    this[match[1]] = match[2];
                }
            } else {
                this.content.push(render_line(lines[i]));
            }
        }

        this.loaded = true;
    }
}


function render_line (line) {
    if (line == '') {
        return '<br>';
    }

    if (line[0] == '>') {
        line = '[cb;' + line;
    } else if (line[0] == '※') {
        line = '[gb;' + line;
    }


    var regex = /\[(?:([brgynpcwoBRGYNPCWO])([brgynpcwoBRGYNPCWO])(-?))?;/;
    var tokens = []
    while (line) {
        var m = line.match(regex);
        if (m) {
            if (m.index) {
                tokens.push(line.substring(0, m.index));
                line = line.substring(m.index);
            }
            tokens.push(m[0]);
            line = line.replace(regex, '');
        } else {
            tokens.push(line);
            line = '';
        }
    }

    var ret = '';
    var color = false;
    for (var i in tokens) {
        var m = tokens[i].match(regex);
        if (!m) {
            var t = tokens[i];
            t = t.replace(/(^|[^"<])(https?:\/\/[^ ]*)(?![">])/g, '$1<a href="$2" target="_blank">$2</a>');
            t = t.replace(/`([^`]*) +<([^`]*)>`_/g, '<a href="$2" target="_blank">$1</a>');
            ret += t;
            continue;
        }

        if (m[0] == '[;') {
            if (color) {
                ret += '</div>';
            }
            continue;
        }

        if (color) {
            ret += '</div>';
        }
        ret += '<div class="text-block f'+ m[1] +' b'+ m[2] +' '+ (m[3] == '' ? '' : 'stroke') +'">';
        color = true;
    }
    if (color) {
        ret += '</div>';
    }

    return ret;
}


$(function () {
    var articles = [];

    for (var i = 0; i < ARTICLE_FILES_LIST.length; i++) {
        var atc = new Article(ARTICLE_FILES_LIST[i]);
        atc.index = i + 1;
        articles.push(atc);
    }

    var article_list_render = new Vue({
        el: '#articles-list-panel',
        data: {
            WEEK_COLOR: WEEK_COLOR,
            articles: articles,
        },
    });

    var article_content_render = new Vue({
        el: '#article-content-panel',
        data: {
            articles: articles,
        },
    });

    download_article(articles);

    // // display state, (*)ST_LIST / ST_ARTICLE
    // StateMachine.state_init();
    //
    // $.ajax({
    //     url: 'Articles/signatures.txt',
    //     cache: false,
    // }).done(function (msg) {
    //     ArticleContentManager.handle_signatures(msg);
    //
    //     // article contents must be fetched after the signature file
    //     ArticleListManager.article_list_panel_init();
    //     ArticleListManager.article_list_panel_mouse_binding();
    // });
});


function download_article (articles) {
    for (var i in articles) {
        if (!articles[i].loaded) {
            (function (idx) {
                $.ajax({
                    url: 'Articles/' + articles[idx].fname,
                    cache: false,
                }).done(function (msg) {
                    articles[idx].parse(msg);
                    download_article(articles);
                });
            })(i);
            break;
        }
    }
}
