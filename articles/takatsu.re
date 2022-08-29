
= cod2e2 - VSCode + Docker開発環境の管理方法の模索

== TL;DR

VSCode@<fn>[takatsu-vscode]のRemote - Containers@<fn>[takatsu-remote-container-ext]extensionを更に使いやすく拡張 + 周辺管理ツールの作成

== はじめに

docker containerに開発環境を構築するメリットは色々とありますが、中でもVSCode@<fn>[takatsu-vscode]のRemote - Containers@<fn>[takatsu-remote-container-ext]extensionは特に便利です (以降では簡単のため、単に"remote-container ext."と表記することとします) 。
何と言ってもこの拡張機能の強みは、@<code>{docker-compose.yml}や@<code>{Dockerfile}を指定し、そのcontainerの中で透過的に開発が出来る点です。
またこれらの定義ファイルが無くても、コマンドパレット経由のオプションを指定することで、予め決められたテンプレートを「イイ感じ」に設定してくれる機能も備わっています。
いずれの場合でも、"remote-container ext."はその設定を@<code>{.devcontainer}ディレクトリ以下に格納します。

しかしいざ使い始めてみると、最初は便利なものの色々と細かい所で「小回り」が効かず、もどかしさを感じてしまいます。例えば

1. @<code>{.devcontainer}配置問題: @<code>{.devcontainer} をプロジェクトのソースコードに含めるかの問題
2. 好みのCLI使えない問題: containerの内部で普段の使い慣れたCLI環境が使えない
3. 複数container使えない問題: 内部に入りたいcontainerが複数あって、どれも使いたい

...などの問題が出てきます。

こういった課題を解決すべく、"cod2e2"@<fn>[takatsu-cod2e2]という簡単なツールを作成しました。今回はこのツールの作成に至った考え方や出来ることなんかを紹介していこうと思います。

== 使いづらさの分析

ツールそのものの本題に入る前に、「はじめに」の項で挙げた課題について、一つ一つちゃんと整理し直してちょっと深掘りしてみましょう。

=== @<code>{.devcontainer}配置問題

そもそも、一般的にこの問題はVSCodeに限った話ではなく、割と根が深いです。
あくまでも @<code>{.devcontainer}は数あるエディタの一つの、しかも極めて個人的な設定に過ぎない訳で、極端な類推すれば .vimrc をプロジェクトのコードに含めたいと言っているようなものです。

ただ、@<code>{.devcontainer}の場合はちょっと事情が異なります。例えばプロジェクトを動かすのに必要なdocker周りの設定（とその中に含まれる処理系やパッケージのversionを）も同時に含まれていることがあり、個人的な「勝手な」設定だと片付けてしまうには少々乱暴な気もします。こういった立ち位置の曖昧さが問題を複雑にしています。余談ですが、実際には両陣営とも一定数いるようです。

さておき、一旦@<code>{.devcontainer}をプロジェクトに「含める派」と「含めない派」に分けて整理してみましょう。まず、含める派のモチベーションです。

 1. プロジェクト中の@<code>{Dockerfile}や@<code>{docker-compose.yml}との二重管理を回避したい
 2. 一元管理することで、開発環境構築の手間を少しでも省きたい (似たような言語の環境設定は再利用したい)
 3. VSCodeのおすすめ拡張機能 (やlinterやformatter周りとの連携) はプロジェクトの開発者間でもまとめて共有したい

逆に含めない派のモチベーションはこんなところでしょうか。

 1. そもそもアプリケーションコードとインフラ環境のコードは明確に分けておきたい
 2. 環境まで共通化してしまったら、「最大公約数的」にmiddle-wareを含む環境設定コードが出来上がり、image sizeの肥大などを招いてしまう
 3. やっぱり@<code>{.devcontainer}に個人の設定や属人的な拡張機能が混入するリスクは防ぎきれない

どちらの主張も凄くよく分かります。ただもう少しよくこれらの主張を精査してみると、@<code>{.devcontainer}の責務が曖昧過ぎる（広すぎる; 割となんでも出来てしまう）ことが原因のように思えます。

以上の分析から分かる第一の方針としてはズバリ、「個人のエディタ設定」と「プロジェクト共通で共有しておくべき設定」を明確に切り分けることではないでしょうか。
一旦「含めない派」の立場をベースとしつつ、各言語の環境設定 + 拡張機能設定を個人で別途管理することを目指した方が良さそうです。

=== 好みのCLI使えない問題

==== containerのデフォルト状態を利用しても良いけど...

"remote-container ext."を立ち上げて、VSCodeの統合ターミナルを起動すると、指定したcontainer内部のshellが立ち上がります。
公式templateから作ったcontainerではデフォルトではbashで、しかもGitコマンドを叩いてみると自動でuserを認識してくれています。生成時の選択を工夫すればzshも使えるようです。これは便利...なのですが、後々問題になってきたりします。
一方で、プロジェクトで既に用意されている@<code>{docker-compose.yml}やら@<code>{Dockerfile}のcontainerを指定すると、大抵素のbash辺りが起動するでしょうか。
或いは、image size削減のためにalpine imageとか使っている場合だとbusybox ash辺りが立ち上がるでしょうか。
当たり前ですが、dev用のimageとはいえ、その手の個人用のCLIツールなんかは大抵installされてはいません。

いずれにせよ、統合ターミナルから起動出来るshellには、当たり前ですが基本は何も入っていないplainな状態のshellが立ち上がることが殆どです。
勿論pureなbashでも十分バリバリ使える人であればそれはそれで良いのですが、多くの開発者は@<code>{.dotfiles}を管理しているのではないかと思います。
fzfやdirenv、deltaやbat等の便利なCLIツールをゴリゴリ入れていて、どうしてもlocalとの生産性は段違いです。

==== だったら、Dockerfile内で各自dotfilesを導入すれば...

こうなるとほぼ解決策は一択で、各自@<code>{.dotfiles}を@<code>{Dockerfile}上の命令に書き足すことを考えるでしょう。
しかし、プロジェクトの方針が前述の「含めない派」であれば、勝手に個人の使い勝手の良いように@<code>{Dockerfile}を書き換えることは不可能ですし、プロジェクトの@<code>{Dockerfile}を一度buildしてから更にそのimage名で@<code>{FROM}したDockerfileを新たに作って...なんて手もありますが、image名が変わってしまったり、プロジェクト側のpackageの更新やら何やらが発生する度に、逐一追従する必要が出てきてしまいます。@<code>{Dockerfile}にinclude命令みたいな機能があれば良いですが、現状では導入されておらず、公式の回答では「Cプリプロセッサで代用してくれ」とのこと@<fn>[takatsu-dockerfile-include]らしいです。

また、もしこの問題をクリア出来たとしても、「プロジェクト側のpackageの更新が起こる度に再buildが必要」になる問題は解決出来るとは限りません。
寧ろ、プロジェクト側の@<code>{Dockerfile}をベースにしている場合、大抵この共通の処理系のレイヤーがより深い部分で構築されてしまっており、非効率です。
以下の図の例だと、Pythonの処理系そのものの更新頻度は高くないですが、packageはそこそこ更新されることが期待され、その度にdocker buildで毎回dotfilesのinstall処理が走ることとなります。

//image[monolithic-docker-image][密結合なdocker imageとbuildの様子]{
//}

「好みのCLI使えない問題」では、その根本原因に「monolithicであること」がありそうです。
この問題を解決するためには、恐らくserviceをもう少し細かく、microに複数切り分けることを意識する方針を取った方が良さそうです。

=== 複数container使えない問題

ではmicroに切り分ける方針はどうでしょうか？
実はこちらもも上手く行かなさそうです。
理由としては単に"remote-containers ext."が現状で単一のcontainerしか指定できないのがまずあります。
仮に将来的に複数指定出来るような仕様に拡張されたとしても、やはりそう上手くは行かなさそうです。

Webアプリケーション開発を考えてみます。
frontendとserverサイドで開発チームが分かれているのならば問題にはなりませんが、小規模チームだったり、個人での開発の場合、一人の開発者が複数の言語や処理系を使って開発するケースがあります。
こういった場合に、例えばPHPとnode.jsを単一のcontainerに格納してしまうと、前述と同様の問題が発生してしまうため、通常は処理系毎に分けて、別々のcontainerとして管理しているケースが一般的です。
今回はfrontendとserverサイドを例にとりましたが、これらは複数の処理系が必要な場合に限らず、複数のmiddle-wareが必要な場合にも同様の問題が発生します。

それぞれのcontainerに同じdotfilesを導入する方針では、前述と同様の欠点が生じてしまいます。
そこで、@<code>{.dotfiles}レイヤーだけ新たなcontainerとして切り出す方針はどうでしょうか？
こちらは処理系やmiddle-wareが幾ら増えても対処可能そうに見えます。
ですが、どうやって特定のcontainer内部から別のcontainerと対話すべきなのでしょうか？

HTTPリクエストを投げて対話する程度ならば、通常のdocker-composeの想定する利用の範囲内に収まりそうですが、CLIベースのinteractiveなやり取りの場合、上手くいくかは未知数です。
なんとなく上手く出来そうな気がしますが、各containerのmountpointを考慮したPath変換等を考慮する必要がありそうです。

//image[multiple-containers-handling-strategy][.dotfilesレイヤーをbaseとした複数containerを扱う戦略]{
//}

== 具体的な解決案

前置きもこの辺にして、具体的な解決案を見ていきましょう。

=== 「@<code>{.devcontainer}配置問題」への対処法

==== ディレクトリ構成上の工夫

「含める」「含めない」問題は、実は「含めない派」に立てばある程度解決可能です。
以下のようなディレクトリ構成と、@<code>{.devcontainer/devcontainer.json}の"*"を以下のように設定することで解決可能です。

//list[devcontainer-directory-composition][.devcontainerのディレクトリ構成][bash]{
./src/   # アプリケーションコードの含まれるディレクトリGit Repositoryのルートディレクトリもこの直下
.devcontainer/
}

//list[devcontainer-json-setting][devcontainer.jsonの設定方法][JSON]{
}

@<code>{devcontainer.json}の設定により、@<code>{docker-compose.yml}をoverrideし、dev環境に必要な部分のみを@<code>{.devcontainer}ディレクトリ以下の@<code>{docker-compose.yml}でカバーする運用をしています。こうすれば、@<code>{.devcontainer}をプロジェクトに含めずに、かつ二重管理のリスクを最小限に押さえることが可能です。

しかし、このままでは@<code>{.devcontainer}はGitの管理対象外のため、version管理はしづらいです。
どうしてもGitの管理下に起きたい場合には、

 1. 上記のディレクトリに@<code>{.gitignore}を配置して、アプリケーションコードの含まれる@<code>{./src}を除外
 2. アプリケーションコード側をsubmoduleとして管理

のいずれかを実行すると良いでしょう。1.の場合には完全に別Repo.としての管理になりますが、全くversion管理しないよりは良さそうです。

==== 再利用性向上の工夫

しかし、これでもまだ完全な解決とは言えません。
「同じ言語やフレームワークを利用したプロジェクト間の共通化・再利用性向上」が達成出来ていません。

この部分に対しては、templateとの比較を行うShellScriptを書くことで対処しました。
具体的には、以下に示すような@<code>{check_external_difference.sh}がこれに当たります。

//list[check_external_difference.sh][check_external_difference.shの概略][bash]{
#!/usr/bin/env sh
set -eu

check_external_difference() {
  # ref: https://github.com/ko1nksm/readlinkf/blob/master/readlinkf.sh
  readlinkf() {
    // 中略
  }
  local readonly SCRIPT_PATH=$(readlinkf "${0}")
  local readonly SCRIPT_ROOT=$(dirname "${SCRIPT_PATH}")

  change_directory_to_repository_root() {
    local readonly REPOSITORY_ROOT="${SCRIPT_ROOT}/.."
    cd "${REPOSITORY_ROOT}"
    return 0
  }

  compare_original_to_derived() {
    local readonly COMPARE_LIST_DIRPATH="${SCRIPT_ROOT}/compare_list/external"
    local readonly SEARCH_FILELIST_PATH="${COMPARE_LIST_DIRPATH}/search_files"

    compare_files() {
      set +e
      # change here with your preferred diff command chain!
      command -v delta 2>&1 >/dev/null
      if [ "${?}" -eq 0 ]; then
        diff -su "${1}" "${2}" | delta --side-by-side
      # default diff part
      else
        diff -s "${1}" "${2}"
      fi
      set -e
      return 0
    }

    enumerate_compare_list_path() {
      find "${COMPARE_LIST_DIRPATH}" -mindepth 1 -maxdepth 1 -type d
      return 0
    }

    for compare_list_path in $(enumerate_compare_list_path); do
      local original_dirpath="./languages/"$(basename "${compare_list_path}")
      local derived_list_path="${compare_list_path}/derived"
      local search_filelist_path="${compare_list_path}/search_files"
      for derived_dirpath in $(cat "${derived_list_path}"); do
        for file_path in $(cat "${search_filelist_path}"); do
          local original_filepath="${original_dirpath}/${file_path}"
          local derived_filepath="${derived_dirpath}/${file_path}"
          compare_files "${original_filepath}" "${derived_filepath}"
        done
      done
    done
    return 0
  }

  change_directory_to_repository_root
  compare_original_to_derived
  return 0
}
check_external_difference
}

なんてことはないコードです。
予め@<code>{cod2e2}で@<code>{.devcontainer}設定のテンプレートを生成する際に、@<code>{./compare_list/external}にPathを記録しておき、@<code>{search_files}に記録された比較対象リストに記録されたファイルのみ、その差分を検出して表示するものです。

こういった緩い差分検出方式を採用することで、以下の効果が得られます。

 1. @<code>{cod2e2}側でtemplateを管理して、再利用可能
 2. プロジェクト側の事情で一時的に追加した部分もtemplateといつでも比較可能
 3. 更に、多くのプロジェクトで採用されるようになった追加要素も、後でtemplate側に「逆輸入」する使い方も可能

再利用性を高めるだけでなく、template自身も漸進的に良くしていく効果がまで得られたのは、一つの大きな成果ではないでしょうか。

=== 「好みのCLI使えない問題」・「複数container使えない問題」への対処法

==== base_shellの導入

こちらは、既に「使いづらさの分析」の項で薄っすら方針は提示しましたが、ズバリ、@<code>{.dotfiles} + shellだけで構成される"base_shell"コンテナを導入することで解決できます。
VSCode公式から提供されているplainなshell image@<fn>{takatsu-vscode-debian-base-dockerfile}@<fn>{takatsu-vscode-debian-dockerfile}をベースに、必要なpackageをapt等経由でinstall、その後@<code>{.dotfiles}を展開する等すれば、作成可能です。
好みのdistroを使えば良いとは思いますが、取り敢えずDebianを使っておけば、恐らくpackageが無くて困るという問題は無いでしょう。
HTTP認証であれば、container内部でhostのGitのcredentialをそのまま使えるのも大きなメリットです。（SSH認証であれば鍵ファイルをマウントすれば通信可能です。）

副次的ではありますが、base_shell containerを一度作るだけで、imageのcacheを用いることで複数の他のプロジェクトでも共有が可能になる点もこの方法の大きなメリットです。

==== serviceの切り分け

前項とも被りますが、base_shell containerと処理系container（ここでは"runtime container"と呼ぶこととします）を分けることで対処可能です。
ライフサイクルの違う複数のcontainerを同時に管理することも可能であるのと同時に、imageのcacheが効きやすくなる点も大きなメリットです。

==== container間の疎通を補助するscript

しかし、この方法にはまだ大きな欠点があり、それはbase_shellとruntime container間の疎通をどう上手くやるかが未知数でした。
こちらは、以下の三つのアプローチを採ることで実現可能です。

 1. base_shell containerでDockerのSocketをmountする
 2. base_shellに各runtimeに含まれるコマンドをwrapするscriptを配置して、@<code>{PATH}を通す
 3. コマンド実行前に引数に含まれるPathを変換する

順に説明していきましょう。

1.のアプローチによって、base_shell container内部からhostの@<code>{docker-compose}コマンドを呼び出すことが可能です。
これによって、base_shell内部から別のcontainerを起動や停止等は出来るようにはなりました。

次に、2.のアプローチによって、@<code>{docker-compose run --rm ...}などのコマンドをShellScriptでwrapします。
ここで例えば、phpコマンドをruntime containerで実行するShellScriptの@<b>{ファイル名}を、コマンド名そのままの"php"としておき、@<code>{"${@}"}などで引数を渡せるようにしておきます。このscriptをPATHの通ったディレクトリ配下に配置することで、あたかもbase_shellコンテナ内でphpコマンドを実行しているかのように使うことが可能となります。

しかし、上記のアプローチだけではまだ完全ではありません。3.

== 使い方の簡単な紹介

clone -> tool叩く

現状使えるのはほぼ PythonとPlantUML。

== 新たな課題点

== おわりに

=== まとめ

TBD

=== 今後に向けて

base_shell二重管理問題
ShellScript + plain-textの保守性が最悪な問題

museum@<fn>{takatsu-museum}作ってます

//footnote[takatsu-vscode][https://code.visualstudio.com/]
//footnote[takatsu-remote-container][https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers]
//footnote[takatsu-dockerfile-include][https://github.com/moby/moby/issues/735#issuecomment-37273719]
//footnote[takatsu-cod2e2][https://github.com/at0x0ft/cod2e2]
//footnote[takatsu-vscode-debian-base-dockerfile][https://github.com/microsoft/vscode-dev-containers/blob/main/containers/debian/.devcontainer/base.Dockerfile]
//footnote[takatsu-vscode-debian-dockerfile][https://github.com/microsoft/vscode-dev-containers/blob/main/containers/debian/.devcontainer/Dockerfile]
//footnote[takatsu-vscode-container-template-list][https://github.com/microsoft/vscode-dev-containers/tree/main/containers]
//footnote[takatsu-museum][https://github.com/at0x0ft/museum]
