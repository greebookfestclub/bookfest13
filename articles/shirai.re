= AIとコラボして人気絵師になる

//image[StableDiffusion_fig1][Stable Diffusion によるイラスト生成例(筆者とのコラボで制作)]{
//}


== 「例のAIが描いた絵です、プロンプトは秘密です！」
「例のAIが描いた絵です、プロンプトは秘密です！」って人間の絵師が自分で描いた絵をイラスト投稿サイトで売り始める未来までは見えた…って、いうツイート@<fn>{tweet0824}が軽くバズってしまったその後の話になります。

前提として、私自身、人間が描く行為は大好きです。
自身も学生時代はマンガを描いてました。
過去形でなく、映像を作るうえではスケッチやキャラクターデザイン、指示書や発明の図など、マンガは業務でも描きます。
そう人間の手で描くイラストレーションを愛しています。
「AIで描けるなら人間は絵を描かなくていい」ってことはないと思いたい。人類バンザイ！

しかしそれはそれとして、業務でのスケッチは心の状態の向き不向きに関係なくサッサと描いてしまいたいですし、
それに「有名・神絵師」ならぬ「有名AI」であったとしても「クオリティの高い絵なら見てみたい」のではないでしょうか？
少なくとも私は観たいと思います。

そして(あわよくば)人間の絵師が「私は有名AIです」といってイラスト投稿サイトで有名になるようなこともできるんじゃないか？という未来です。
もう来月ぐらいには一般的になるんじゃないかなと。
そこまで行くと人類はまた新たなステージに立てると思いますので、本稿では「AIとコラボして人気絵師になる」、そのための技術開発を目標に調査研究してみたいと思います。



== 人工知能で絵を描くための前提知識と用語

さて本題に入ります。「最近は人工知能で絵を描くことができます」というこの1行のために、何本かの論文を読む必要があります。
オッサンが鉛筆を削ってデッサンをゼロから学ぶよりははるかに楽なので付き合ってください。

本稿では「Stable Diffusion を使っていい感じの萌え絵の素材を描かせるためのプロンプト（呪文）」を理解する、という技術の根幹と使い手としての入門を同時に達成するために、
数式を一切使わずに、思い通りの結果を得るために必要な研究を解説していきたいと思います。
その過程で DALL-E や MidJourney 、Stable Diffusion の背景に使われている技術が面白いな、このスピード感はついていかないと置いていかれるな…そう思えればそれでOKです。

=== 人工知能で絵を描く研究開発の歴史と背景
「人工知能で絵を描く」と一言で表現しても、いろいろな要素が出てきました。古くは画像理解や、GA（Generative Algorithm；創発的アルゴリズム、もしくはGenetic Algorithm；遺伝的アルゴリズム）、さらに画像生成分野ではNPR（Non-Photorealistic Rendering；非写実的な画像生成）、さらにレタッチ分野では Photoshop などを長年開発している Adobe社がとても強いです。Adobe には Adobe Research という研究グループがあります。彼らは画像を直感的な作業で改善するというシンプルな出発点から、写真のフィルタリング技術、背景の除去などを長年開発してきました。最近では、動画の音声を文字起こししてくれたり、簡単なレイアウトを描くだけで神々しい背景画像を作り出してくれたり…となかなか便利そうな技術が国際会議やカンファレンスなどでのデモで発表されています。

一方で、Adobe のような企業は製品を売って利益を得る企業です。独占的なAIの開発はある目的にとっては良いかもしれませんが、オープンではなく、多様な用途には向いていませんし、ブラックボックス化する技術に対して「人間たちが勝手に恐怖を抱いてしまい、逆に普及しない」というジレンマもあります。
そんな背景もあって、2015年にテスラ社のイーロン・マスクを始めとする実業家・投資家によって設立されたオープンな人工知能研究機関が「OpenAI」です。
OpenAI は、人工知能を研究する非営利団体で、人類全体に「害をもたらすよりは、有益性があるやりかた」で、オープンソースと親和性の高い人工知能を注意深く推進することを目的としています。
これまで GPT (Generative Pretrained Transformer) のような自然言語処理、言語のメタ学習が可能なアルゴリズムを発表していましたが、最近は DALL-E という「文字列から（意味を解釈して）画像を生成するAI」を発表しています。
文字列から画像を生成するだけであれば、その他にもいくつかの人工知能や類似のシステムは存在しましたが、その改善版「DALL·E 2」においては、自然言語による記述からリアルな画像やアートを作成できる新しいAIシステムとなりました。

そして本稿書くこの時点で明言しておきたいことは、DALL-E の登場以前は、「コンピュータが描いた絵」に人々は驚きこそすれど、人気になったり、親しみを得たり、もしくはそこに熱狂してお金を投じたりといった対象ではなかったかもしれない、という点を指摘しておきます。

たとえば2021年ごろに熱狂的なブームとなった NFT（Non-Fungible Token）アートは、「代替不可能なトークン」つまり、偽造や改ざんが難しいブロックチェーン技術によって、ネットワーク上の画商やその市場を開拓しました。100～1000といった有限なセットで発売されるため、「有名な絵師さんによる、限定性」が商品の価値を高める要素になります。
一方では大量に記号的なアートを生成する中で「私はこれが気に入った」という自己主張やコミュニケーションとして使われて行きます。
つまり、有限な組み合わせによって生成されるアバター市場や、GAによって生成されるコンピュータによる画像生成、そしてその趣味嗜好（preferred；好ましい）モデルを獲得する事には、メタな技術としての強みが生まれます。

=== CLIP 潜在性を用いた階層的テキスト条件付き画像生成法
さてここでは、2022年4月にOpenAIの Aditya Ramesh らによって書かれた「DALL·E 2」についての論文「Hierarchical Text-Conditional Image Generation with CLIP Latents（筆者による邦題；CLIP 潜在性を用いた階層的テキスト条件付き画像生成法）」(https://arxiv.org/abs/2204.06125)を平易な日本語で関連用語とともに解説していきたいと思います。
私自身は知能システム科学で博士を取得したのですが、この分野は査読などの経験はないのでシロウトです。紹介するのは最新の研究なので間違っていることもあるかもしれないです。

==== Diffusion モデルと関連する専門用語

解説を通して専門用語がいくつか出てきます。
このあとの急速な進化が予感される「AIで絵を描く技術」、特に Stable Diffusion で覚えておきたい用語は「Diffusionモデル」、そしてその背景にある「CLIP」と、「GLIDE」とそれらを支える技術でしょうか。
一つ一つ理解しやすいステップで、簡単な言葉で説明していきたいと思います。

==== CLIP : 文字と画像をつなぐモデル

まずOpenAIの先行研究として CLIP (Connecting Text and Images；テキスト画像接続) という手法があります。
「CLIP」は「WebImageText」という画像とそれを説明するフリーテキストのペアでできた4億のテキストと画像のペアを対象学習（Contrastive Learning/コントラスト学習）で学習させた自然言語教師型画像分類モデルです。
「画像とテキストの関係、類似度を測ることができる巨大な表」とイメージするとよいでしょう。
CLIPもunCLIPなど複数の改善バージョンが存在しています。

多様なタスクに対して「ゼロショット」、つまり何度もショットを取らない、タスク特有の学習内で、事前学習データだけで学習したモデルを直接タスクに転用することができるという特徴があります。

CLIP のような既存手法では「セマンティクス」（≒意味、この場合は画像の領域に描かれた意味）と、「スタイル」の両方を捉えた頑健な画像表現を学習することできます。
この「意味」と「表現」の2つを画像生成に活用することを考えて、「キャプション」（注釈として与える文字列）を指定してCLIP画像への埋め込みを生成する「事前のモデル」（prior；事前分布）と、画像埋め込み(embed)を条件として画像を生成する「デコーダ」の2段階によるモデルを提案しています。

つまり画像の表現スタイルを明示的に文字列で指定すれば、フォトリアリズムな表現とキャプションで指定された「意味の類似性における差を最小限に抑えつつ、画像の多様性を向上させられるという」手法です。
さらに、CLIP に結合されている埋め込まれた意味空間は、ゼロショットで（つまり事前の知識やゴールの設定なく）言語誘導的な画像操作を可能にします。デコーダには後述の「拡散モデル（Diffusion Model）」を用います。論文によると、事前分布には自己回帰モデルと拡散モデルの両方を用いて実験し、後者の方が計算効率が良く、より高品質なサンプルを生成できると報告しています。

このあとに続く研究でも CLIP の手法 が使われています。
そして、当然なのですが、この CLIP で学習したものが、このあとの結果に大きな影響を及ぼします。

==== DDPM: ノイズ除去拡散確率モデル
「Diffusionモデル」は「Stable Diffusion」の「Diffusion」の語源にもなっている「拡散」のことです。
画像生成手法の一種で、DALL-E2では GLIDE という Diffusionモデルによる Text-to-Image生成モデルが使われています。
フルネームでは「ノイズ除去拡散確率モデル（Denoising Diffusion Probabilistic Models）」というモデルで、DDPM と呼ばれます。

==== 逆マルコフ過程と超解像技術（SR）
DDPM を理解する上では、2015年ぐらいから研究されている超解像（Super Resolution; SR）の技術を理解せねばなりません。
まず画像に微小なノイズを足していく過程を考えます。これは「マルコフ過程」として表現できます。
マルコフ過程とは、未来の挙動が現在の値だけで決定され、過去の挙動と無関係であるという性質を持つ確率過程です。
基本情報技術者試験に出そうな問題で例えると、「天気は1日前の天気にのみ依存するマルコフ過程である」とします。晴れの翌日は40％で貼れ、40％で曇り、20％で雨、といった確率で変化し、曇り→雨、雨→曇りなど全てのとりうる状態が確率で表現できて、前日より前の天気には依存しないとするとします。

ここで「各ステップごとに画像データにノイズを加えていく過程」を考えます。
これは「破損プロセス」と表現できます。とある現在の画像に対してノイズを加えていくのであり、それぞれのステップは過去のすべての行為とは無関係といえます。
DDPMでは、このような思い切った考えで、ノイズから実データを復元するようにモデルを学習させていきます。
ノイズはガウス分布という一般的な発生の確率に沿わせます（違うノイズの与え方もあります）。
そして、その逆工程もマルコフ過程として表現できるので、この逆の工程を学習させることで、ガウス分布ノイズの塊から画像を復元できる学習を獲得できる…という魔法のような手法です。

論文としてはSR3と呼ばれる手法の「Image Super-Resolution via Iterative Refinement（筆者邦題；反復精製による画像の超解像化）」（https://arxiv.org/abs/2104.07636）を引用しておきます。

=== 超解像技術を用いたノイズ除去拡散確率モデル条件付き画像生成
ちょうど絵を描く工程に例えると、真っ白な紙にポスターカラーで絵を描いていくような工程があったとして、その完成図から工程を逆にたどるような条件で学習を行えば、ノイズの塊から目的の絵を描くことができる、ということになります。

「猫を描こう」と思ったら、完全なガウスノイズが乗った猫→ノイズを減らした猫→ノイズがない猫の画像を用意して、その逆工程、つまりデノイズをピクセル単位で獲得していきます。
ガウスノイズは近隣のピクセルとは異なる、色が派手な変化を起こしますので、この理想的な分散を持ったノイズを除去できる学習は、目的の絵を生成できるはず、ということですよね（何故なのか、はここでは一旦置いておきましょう）。

この「猫」を画像ではなく、猫のミーム、つまり猫を表現するようなWebから拾ってきた画像で学習させたものが「超解像技術」＋「ノイズ除去拡散確率モデル」＋「条件付き画像生成」となります。

確率的ノイズ除去処理によって超解像、つまり本来なかった倍率まで解像度を上げていくという技術ですが、特に顔や自然画像に対して高い性能を発揮します。

そして「なぜこのノイズの除去で画像が生成されるのか？」という点については、また別途研究されていてわかっていないことも多かったようです。
ガウスノイズ以外のノイズでも動作することが報告されています。
例えばColdモデル、つまりランダム性がないDiffusionモデルも存在しますので、より制御された絵作りには別のモデルが選択できる可能性もありますね。

 * Cold Diffusion: Inverting Arbitrary Image Transforms Without Noise (https://arxiv.org/abs/2208.09392)

=== 潜在変数と拡散モデルを使った描画
さて、超解像技術の場合は「もともと描かれていた画像に対して、解像度を上げていく」という条件付けですが、この潜在変数を CLIPの潜在性 (CLIP Latents)に注目し、画像と言語の関係に埋め込まれた意味（セマンティック）にしたものが GLIDE です。

35億パラメータで、このDiffusion(拡散)モデルを学習し、もともとの意味としての画像は64x64ピクセルのジグソーパズルのようなピースでのセマンティックで作ります。
このセマンティックは「デコードするといい感じの画像を生成する潜在変数」と理解することができます。

このままでは64x64ピクセルですが、ここに「超解像技術」を組み合わせることで、最終的にフォトリアルな画像として、256x256ピクセルで画像が生成できます。
がんばれば512x512など画像サイズを上げていくことができますが、さらに大きなメモリ空間が必要になります。
また1ピクセルづつ、1工程づつデノイズを行っていくので、非常に効率が悪い処理をすることになります。

なお、レイアウト上、複雑になるとうまく生成できない問題がありますが、領域を指定することで人間がさらに補っていくことで完成させるという手法も並列して存在します（過度な脱線を避けるため、本稿ではワンショットで、領域指定をしない手法を中心に紹介していきます）。

=== GLIDE：テキストガイドによる拡散モデルを使った写実画像生成・編集
OpenAIの「DALL·E」から約1年、進化モデルは「テキストガイドによる拡散モデルを使った写実画像生成・編集を目指したアルゴリズム『GLIDE』」として発表されます@<fn>{GLIDE}@<fn>{GLIDE-Paper}。

拡散モデルによってノイズを徐々に除去する際に、分類器の出力誘導を行い、目的の方向に生成を「誘導する」ことができます。

=== DALL·E 2：CLIPと拡散モデルを組み合わせた2段階モデル
さて、やっと「DALL·E 2」に辿り着きました。
画像の埋め込みを生成する「事前分布 (prior)」と、埋め込みから画像自体を生成する「デコーダ (decoder)」によって、構成されます。
自然言語による記述文と、「デコーダ」は、CLIP 画像埋め込みから生成します。
画像を説明する説明文が言語モデルを介して「embedded特徴」に変換されます。
学習と生成は以前と同じように進行しますが、ノイズ除去の追加入力としてヒントを提供するテキスト入力が入ります。
画像を受け取ってembedded特徴を生成するので、「CLIP を反転しているものだ」と考えればよいかもしれません。


== MidJourney を使ってみる

さて技術は使ってみなければその本質はわかりません。
まずは 「DALL·E 2」と「Stable Diffusion」の間に生まれた「MidJounery」を使って試してみました。

 * https://www.midjourney.com/home/

Discordをインターフェースとしており、25枚は無料で生成することができます。会員登録していれば商用利用も可能とのこと。
ちょうど仕事でコンセプト画を描かなくてはならないので、例として「high school girl in metaverse（メタバースにおける高校生の少女）」などをプロンプトにしています。


//image[StableDiffusion_MidJourney][(左)"high school girl in metaverse"／(右)超解像化された結果]{
//}

細かい造作を入れていきたいので、「REALITY manga style smiling avatar female with wings and cat ears（REALITY, マンガ風, 笑顔の, アバター女性, 羽と猫耳付き）」で探索してみます。

そもそも ``REALITY'' が何のことなのか、知っているのかどうか？でいうと、そもそもこれは判定が難しいワードですね。
結果としては雰囲気はないこともない（実験したのは2022/8/4ごろです、今後変わる可能性もあります）。

いろいろ試してみるとVTuberについての知識はありそうなので、今度はスタイルの指定に使ってみます「vtuber style female avatar singing（VTuber風, 女性アバター, 歌ってる）」としてみます。

//image[StableDiffusion_MidJourney2][(左)"REALITYマンガ風微笑みアバター 女性 翼と猫耳付き／(右)VTuber風女性アバター 歌ってる]{
//}


以上のように「有効そうな単語」を探り、でき上った画像から良さそうなものを選んで、それをシードにしてさらに画像を探求していく…という工程をDiscordのチャットを使って超高速に進めていきます。


=== MidJourney は仕事につかえるかどうか

「MidJourney は仕事につかえるかどうか？」という視点で判定してみます。
まずは、形容詞で「赤い髪」とか「青い髪」といった指定ができる要素であればよいのですが、
裏側で動いているアルゴリズムやモデルが謎すぎます。
おそらくCLIPが動いており、word2vecのようなワードベクトルで指定することである程度の類似度やベクトルを指定できる可能性があるので、学習している対象が日本のアニメやマンガではなく、欧米文化の映画やスーパーレアリズムが多く、インパクトのある背景画は生成できますが、安定した萌え絵には使いづらい。
例えば「日本」に関係する画像はいつも富士山とか五重塔のような建造物が現れてしまいます。
また人物に関しては、いつも出てくる人物の雰囲気がいつも同じなので、いくつか画像を生成すると『ああ。MidJourneyだね』という感想を抱いてしまうと思います。


今後モデルの改善が期待されますが、改善されることで結果も変わってしまう可能性があります。安定した結果を出すには難しい。
一方では、超解像技術のおかげもあって、ディテールについては非常に印象深い仕上がりをするので、
「企画者としてのコンセプト画像の検索」という使い道ではいけるように感じました。


=== MidJounery が切り拓いた「AI画像生成というエンタメ」

MidJounery は仕事には使えないかもですが、DiscordをインターフェースとしたAI画像生成体験は、
「MidJourneyはエンタメ体験として成立している」という印象を得ました。
GPUや Discordを使ったUXとしてのターンアラウンドの処理のテンポも重要で、「お金払う価値はあるかも」と思わせる体感があります。
何より周りの人たちが一生懸命画像を生成しているのを見るのは、なんだか熱狂を感じます。

ところで「お絵描きAI」は人類にとって何者なのでしょうか。
「思い通りの絵を描かせたい」という人類、つまり「攻略したいゲーマー(人類)」にとって、遊び相手になってくれる相手でしょうか。

少なくとも、デジタルイラストレーションの世界で勝ち負けで勝負をする相手としてみると、「勝敗をつけるよう場所で戦ってはいけない相手」ではない感じがします。
「そもそもアートに勝敗はありませんよね」という立場を守るのが人類にとって幸せな選択になるでしょう。

DALL-2Eでは修正して「一緒に絵を描く」という貢献が求められましたが、MidJourneyではAI神絵師に呪文を唱えるゲーム的な体感があります。
ものの数秒で生成されてDiscordでメンションされるのですが、これは反応速度が高いGPUが搭載された演算が必要そうな感じがしますし、
その反応のテンポのよさに、コストを払ってゲーム的に楽しみたい人もいそうです。また攻略性だけでなく、癒しにもなります。

例えば「今まで見たことがない癒し猫画像で癒されたい…」といった需要があると思いますが、現実空間で撮影された画像には限界があります。
「どこか遠い宇宙を背景にしたファンタジーイラストで、癒される猫の画像を見たい…」という要求に対して、MidJourney が生成する画風やモチーフはフォトリアルやスーパーレアリズムとしては印象的です。


=== プロンプトエンジニアリングの必要性

MidJourneyは他の人が打ち込んでいるプロンプトが見えているので、エッチな画像をひたすら作り出そうと頑張っている人もいます。
それがプロンプトの改善の参考になったりもして、何というか「何でも描いてくれる絵師さんにひたすら性癖をこすりつけているキモい空間」でもありますが、
一方では、目的の絵を生成させるプロンプトを見つけ出すプロンプトエンジニアリングの必要性を感じます。

そもそも英語の prompt はラテン語の promptus（前へ取り出した）という語で、語源としてはラテン語 promo（前へ取り出す）⇒ ラテン語 pro-（～の前へ）＋emo（取る）が語源。つまり即座に emptus(手に入れる)という意味で、人類が神様にこの神絵師にうまく絵を描いてもらえる詔（ミコトノリ）ではあるけれど、その神主になって即座に入手する技術も「誰にでもできる技術」ではなくなっていくことが何となく感じられます。

このあとの「Stable Diffusion」では、内部のモデル理解を含めて、よりよいプロンプトについても探求していきます。

== Stable Diffusion


== Stable Diffusion：潜在拡散モデル

Stable Diffusion は  ドイツの大学系研究所「CompVis」（https://github.com/CompVis）、 Stability AI（https://stability.ai/） 、LAION（https://laion.ai/）の研究者やエンジニアによって作成されたテキストから画像への潜在拡散（latent diffusion）モデルです。

このモデルは 、frozen CLIP ViT-L/14 テキストエンコーダを使用し、テキストプロンプトでモデルの条件付けを行います。
// 860MのUNetと123Mのテキストエンコーダを持つこのモデルは、
比較的軽量で、コンシューマー向けGPU、10GB以下のVRAMで動作し、512×512ピクセルの画像を数秒で生成します。

Stable Diffusion は64x64のガウスノイズの潜在変数シード(Latent Seed)を与えてノイズ除去超解像化を行う過程に、ユーザの与えたプロンプトを CLIP を通して生成したテキスト埋め込み(77x768)潜在変数に沿うように誘導するという手法を使っています。
簡単に表現すると、人間が言語で与えた意味（セマンティック）を77x768の次元の単語→ベクトルに変換する辞書に渡して、その辞書に近いベクトルに沿わせながらノイズ→512x512画像にノイズ除去しながら超解像化しているとあのような画像が生成される…という魔法になります。

// ジェネレーティブAI開発者のKatherine Crowson氏による条件付き拡散モデル（conditional diffusion models）、
// Open AIによるDall-E 2、Google BrainによるImagen、その他多くの洞察と組み合わせて構築されたもので、任意のテキスト入力から写真のようにリアルまたはアーティスティックな画像を生成することができます。

 * High-Resolution Image Synthesis with Latent Diffusion Models (https://arxiv.org/abs/2112.10752)

// 画像の一部を塗りつぶしてテキスト入力すると、その箇所だけテキスト通りに描き加えることができます。
// しかも、描き加えたその箇所は、周囲の文脈のスタイルや照明に応じた影や反射を含み、周囲と調和した画像が合成されて生成されます。

=== MidJourney と Stable Diffusion の根本的な違い

テキストから画像を生成できるモデル、しかも MidJourney のような商用利用ベースのブラックボックスではなく、
ソースコードも学習モデルも無料で利用できる、しかもそのモデルが非常に高品質な結果を生み出せる、というのがStable Diffusionが有名になったポイントですが、
ここまでの簡単な解説で、超解像技術 → 拡散モデル → GLIDE → DALL·E 2 → MidJourney → Stable Diffusionという、地道な技術の進歩が超短期間でおきていること脅威でもあります。

そして、数日でトレンドを塗り替えられてしまった MidJourney。
この背景にあるのは「拡散モデル」の発展であり、学術系研究者グループや、AIスタートアップが知見と結果を出し合いながら、あっという間に私企業によるサービスを塗り替えていきました。
そしてこの技術は画像生成だけではなく、言語や音声など他のドメインでのデノイズや創作の質向上が期待できそうです。

== Stable Diffusionで絵を描くテクニック

さて Stable Diffusionを使って絵を描いてみます。

ここから先はちょっと真面目に「良い生成結果」と「人気絵師になる工程」を残していく意味もあって（また、紙面ではあまりに量が多いので）、pixivさんのほうに画像を置いてあります。

使い手の進化の過程がわかるので、まずはこちらをフォローしていただくのが良いかなと思います。

 * https://www.pixiv.net/users/1355931/illustrations

//image[StableDiffusion_PixivShirai][筆者の実験用Pixiv(短縮リンク) ivtv.page.link/PixivShirai]{
//}

お手元のスマホでこちらのQRコードを使って見ていただくのもよいかと思います。

=== 注意事項

以下のプロンプトについては解説目的で共有しますが、一部伏せさせていただきます。
特に商標に関わるキーワードと商品性が高いキーワードがあります。
そして「萌え絵」になると、性癖が露になるキーワードが多くなります。
もしどうしても知りたい、という方は pixiv上からコミュニケーションしていただくか、Twitter@o_ob にてお問い合わせください（呪文を忘れてしまっていたらごめんなさい）。


== Stable Diffusion - Dream Studio による探求

皆さんお待ちかね、Stable Diffusionによる作例です。自分は Dream StudioというUIサービスを使っています。

 * https://beta.dreamstudio.ai/

//image[StableDiffusion_DreamStudioUI][DreamStudioのUI。プロンプトを入力して「Dream」ボタンを押すだけ]{
//}

=== 少女画を描いて表現を探ってみる

作品「ちょっとこわいけど怖くない」ちょっとこわいけど怖くない妙な表情が出せたのでうれしい。一方では腕がとんでもないことになっているので怖い。

//image[StableDiffusion_DreamStudioUI][少女画を描いて表現を探ってみる]{
//}

鼻がない画風もしっかり学習されている

とある画風を指定すると比較的簡単に生成できます（後述）

これらの画像をどんなプロンプトで呼び出していくのか？が技術になりますね

=== 「実際しないフィギュア」シリーズが人気になる

//image[StableDiffusion_DreamStudioUI][「実際しないフィギュア」シリーズが人気になる]{
//}

「実際しないフィギュア」のシリーズが（筆者の予想に反して）人気でした。

「Chibi Zombie Fighters」


=== 不可能な絵を描いてみる

人類最初期の壁画であるラスコー洞窟に初音ミクを描いてみる挑戦をしました。

"Illustrated brown black Hatsune Miku is dancing with deer and ox in galaxy, by "Grotte de Lascaux", aged prehistoric wall painting strong paint brush yellow, brown, black. trending on national geographic, wide camera"初音ミクが鹿や牛と一緒に銀河系で踊っているイラストの茶色と黒、「ラスコー洞窟」による先史時代の壁画で、黄色、茶色、黒の強い絵筆を使用。


//image[StableDiffusion_DreamStudioUI][ラスコー洞窟に描かれた初音ミク]{
//}

「ラスコー洞窟に描かれた鏡音リン・レン」

どんなプロンプトを与えられるかについては、考古学的知識が必要ですし、やはり生み出されたノイズ画像から、目的の画像をゴールとして取り出すのは、我々人類なのです。そしてそれが本当なのかどうか、真偽性はともかく、人々にとってどんな魅力や意味を持つのか…我々人類は、メディアと芸術に対して、より真剣に考えていく機会を得たことになります。



=== カメラやフォーカスの指定

//image[StableDiffusion_DreamStudioUI][カメラやフォーカスの指定]{
//}

演出のためのブラーや光、フォーカスなども指定できます。

2449563745 a posing smiling fantasy girl  by Grand Blue Fantasy  matte painting  focus blur trending on pixiv HQ

1132913758 a posing sweet smiling Kentauros Hatsune Miku    by Grand Blue Fantasy  matte painting  focus blur pastel trending on pixiv HQ


=== レタッチテクニック

レタッチはしたほうが良いと思います

いくつかの作例とともにレタッチの重要性を語っておきます。

//image[StableDiffusion_DreamStudioUI][レタッチテクニック]{
//}

Alfons Mikuぼかしていますが、そこにはヤバい指がいます

「Gundamiku」ガンダムfeat.初音ミクというポップアートの権化みたいなプラモを作ってみました。自分がそのプラモを撮影するならこういう効果を入れるだろうなあと

「神々しい表現」意外と西洋美術が好きそうな宗教画とかのスタイルはたくさんあるので、和洋折衷で新しい表現演出を開拓するにはいいかもしれないですね。

レタッチしないけど味わいある作品・使い方

金曜ロードショー「耳をすませば」を見ながら描きました。指が微妙ですが雰囲気はいい

こちらも「無題」スタイルだけが作品になる

「習作」花を描くのが大変なのですが、AI画像生成は得意ですよね。

「帰ってこないあのひと」こちらも 金曜ロードショー「耳をすませば」放映記念。赤いドットと背景のライティングはPhotoShopでのレタッチです。

「ひるねするおくさん」マンガ描線についてはこれだけで語りたい要素がありますが比較的うまくいった事例


以上になります。サービス側でどんどん改善されていくと思いますので、これからも期待です。何かおもしろいスタイルがあったらTwitterで教えてください！

==== おまけ：Stable Diffusion を 手元で動かしたい

 * https://research.google.com/colaboratory/local-runtimes.html
 * https://huggingface.co/spaces/stabilityai/stable-diffusion
 * https://huggingface.co/CompVis/stable-diffusion-v1-4


== 結論：人気絵師になって分かったこと

何が大変って、人の期待と性癖を毎日こすりつけられる。
これに日々耐えてる絵師さんってほんとうにすごい

「おもしろいだろうな」という気持ちで描いても、自分はそのジャンル極めたくないと気付く

「描きたいものを描く」のと
「求められて描く」のは異質
例えば
ミク x ガンダムコラボが需要あることはわかった

大河原邦男だろうか
カトキハジメだろうか
出渕裕だろうか

そもそもサービスに支払っている計算機コストを上回る、ファンからのお布施が入ってこなければ「職業：神絵師」が成立しない。
手元で Python環境やGPU演算環境をつくれば少しは安くなるのかもしれないですが、それでも10円でもお客さんからお金をもらうっていうのは大変なことです。

今日時点の結論：ガチャのイラストを描く絵師さんはすごい。
とはいえ、商品性を学習させていくと、すぐに状況はアップデートされる気もする。
たとえば顔アイコンやLINEスタンプなどは学習しやすいかもしれない。商品性をアノテーションしてくことで…

問題は「上手い」とか「下手」とか、人間は美醜に対して真剣になればなるほど「美しさに対して不寛容である」という点です。ソロで描いているだけであれば、描いている本人がよければそれでいいのですが、SNS時代においては、(1) 本人が好きで描いているのであればそれでいい↓(2)上手い人が視界に入る↓(3) 自分の絵が恥ずかしくなるといったサイクルに常に晒されます。これは実はSNS時代に限ったことではなく、同人誌時代も同じことですし、昭和の時代の漫画家であれば、雑誌や投稿の上で戦っていました。このステップには続きがあります。(1) 本人が好きで描いているのであればそれでいい(2)上手い人が視界に入る(3) 自分の絵が恥ずかしくなる↓ （ここで折れてしまう人もいるのですが）(4) うまい人の絵をみて学ぶというスイッチが重要かもしれません。この人うまい！と思うだけならいくらでもいるのですが、ああ！こういう表現があったか、こういう構図、こういうシチュエーション、こういう描線…この瞳はどうやって描いているのだろう…？といった探求心と向上心がジェラシーよりも上回ることが大切だと思います。単に上手な絵を蒐集している人と、手を動かして描いている人の違いはそこにあるかもしれませんし、その2者の違いはとても大きいです。





====  余談：#StableDiffusion が禁止タグに設定される未来
「思い通りの絵を一通り描けるようになる」をマスターすると、 次は「Pixivが #StableDiffusion を禁止タグに設定する未来」が見えてきました。

えっちな画像を安易に大量生成して、性癖丸出しのタイトルとともに、比較的クオリティの高い絵をpixivのタイムラインに乗せる人が大量に出てくる未来がみえてきます。そもそもpixivでは「オリジナル画像」というチェックボックスがあるのですが、Stable Diffusionを使って書いた絵が「オリジナルではない」という主張をするのは難しいと思います。

そもそも「二次創作である」とすると、「○○の二次創作です」という主張をすることで「○○が好きな人」に刺さるという構造がありますが、「二次創作ではない、これは（○○がいいな！と思って）インスピレーションを受けたが、オリジナルを目指している」という「オリジナル」はいくらでもあるし、Stable Diffusionにおいては潜在変数の組み合わせでしかないでしょう。

簡単に表現すれば「人類の美学とモラルが問われている」という1行でしかないのですが。
具体的には「人間が手を動かして描いた絵以外は認めない！」という原理主義の絵師さんと、そのファンの方々、そして「え？ツールとして使うかどうかとか含めて技量では」という革新主義の方々の認識の違い。さらに、絵を描く側の意志とは全く関係なく、勝手に入手した画像からモデルを獲得しようとする方々などなど。

似たような問題は今後リリースされるサービス全てについて言えると思います、また日本のこの分野においては、勝手に善悪のラインを引いて警察行為を行う人もいるので、どのあたりにモラルラインが引かれるのかは興味深い点ではあります。

// 2022年8月29日にリリースされた「」でも起きているようです。


その潜在変数の組み合わせをどのように詠唱させたか？という点では、実際の絵のデッサンでいえば、エンピツの削り方、その鉛筆を制御する筋肉の鍛え方、光の捉え方、面や空間の捉え方…そしてデジタルイラストレーションでいえば、構図、彩色、表情などに現れてきます。どれだけ多くのスタイルを知っているか？これは美術史なり、美術書なりを読んでいる人のほうが有利です。日本語ではなく英語で指定したほうが良いですし、ベクトル空間として表現可能な画風を英語で指定できさえすればよいので（具体的なキーワード例は後に紹介します）、有効なキーワードを知っていることが大事です。むしろここを人間と対話的に学習させるインタフェースなども需要が出てきそうです。つまり「AIが美しい絵を描くなら人類は絵を学ばなくていいか」という命題は、「学ぶ必要がある」ということであり、「描かなくていいか」は実際の絵の具やデジタルイラストレーションの過程と同じように「必要があれば道具として使ったり学んだりすればいい」という事になるかと思います。またメディア芸術史的な視点でStableDiffusionを観察すると、日本のマンガ、アニメーションやゲームイラストに出てくるようなモチーフは、比較的記号化が進んでしまう要素であるという危惧もあります。

具体的には表情演技です。でもこれは解決する気もします。



=== 筆をおく前に：「萌え絵を書いてください」というお願いの今後

「萌え絵を書いてください」というお願いは比較的短期間に獲得可能であり、猥褻図画もしくはそれに準じる画像をひたすら蒐集・消費したいという目的のサイクルは確実な需要がある一方で、ある程度分離、カテゴライズ、カーテンなりR-18なりで別けることで双方に健全な進化成長を促すことができるようにも思います。

描き手、クリエイターとしての感覚として、自分の画力や筋力（気合の入った絵を何時間も高速に描き続けるには筋力が必要です）に関係なく、上手な出力ができるようになると脳が発火していくことを感じます。つまり人間の脳は絵を描きたい。表現したい、表現を探求したい。これは単体の脳でも感じます。それを脳の外に画像として放出することで、他者の評価や需要を満たす、そしてもっと上手にAIを使いこなす人々にジェラシーを描いたり、その技術を獲得するためにお金を払ったりする人も出てくると思います。つまり冒頭に書いた(1) 本人が好きで描いているのであればそれでいい(2)上手い人が視界に入る(3) 自分の絵が恥ずかしくなる↓ （ここで折れてしまう人もいるのですが）(4) うまい人の絵をみて学ぶ

このサイクルであることは全く否定できません。もちろんこのサイクルに Stable Diffusion というアルゴリズムや関連のサービスが入ってきました。技術の思想はDALL-Eを産んだOpenAIが「企業の独占ではなく」という立ち位置で生まれていることからも、単一企業の独占ではありませんが、賢い投資家たちは「人類が脳汁を発したり、お金を出してでも手に入れたいと思うもの」に対して投資行動をしていくと思います。規模の大小はわかりませんが、これからもAI画像生成技術には一定の注目があると思います。

一方で、このSF小説（としておきます）をここまで読んだ人はお気づきだと思いますが、 Stable Diffusion にはDALL-E2にあったような「任意の場所を描きなおす」というプロセスはないようです。実際に、これらの絵作り工程では、ポージングやレイアウトを直接指定したいことはたくさんありますし、いちばん難しいところは「指と顔の表情」の指定です。顔は記号的な表現でよければワードで指定できるとは思います。でも絵画で表現したいような複雑な表情は難しいですし、これを言語で表せるならそれこそ絵画の終わりを感じる気もします。またアニメーションで表現したい動的な表情変化、表演技に特化した対話的なUIなどもあるかなと思います。この辺はCGxAI分野の研究者のネタでしかないので、学会などでお話しできればなと思います。

// === 人工知能とともに絵を描くという行為が人類にどんな影響を与えているか
// ここまで、人工知能とともに絵を描くという行為が人類にどんな影響を与えているか？という視点で技術解説スタイルで作文をしてきました。

絵を描く必要があるかどうか？学ぶ必要があるかどうか？については実際に体験して本気で画像を作ってみるとわかることがあると思います。そこには明確な技量が必要であり、若い人や経験がある人にとっても、一度は体験しておくべき衝撃があると思います。そしてそれは、「エンピツをもって絵を描くことの意味」とほぼ同意ではないでしょうか。つまり「その技法で頑張ること」の意味を自分が理解すれば、それにこだわる必要はないが、やったことがない状態で良し悪しを判断するのは野蛮なことである、というぐらいの話でしょうか。もちろん「苦労したほうが良いものが描ける」という視点もあると思いますが、技法としての苦労と、表現としての苦労、表現者の表現したい画風とはそれぞれ異なってよいという見方ができるかどうかです。単体の脳にとって、美的な何かを探求する、自らのモチベーションと向き合う、という行為は大変刺激のある行為であり、数億～数十億といった、現在の地球人類の個体数の総和に匹敵する数のスタイルを学習したAIと対話することの意味は、まさに人類がいままで描き残してきた画像メディアの歴史的集約との対話を行っていることに他ありません。神絵師との対話は「こういう画像を得たい」という人間のモチベーションと向き合うことに他ありません。むしろ人類が「画像などいらぬ」という存在であれば、AIアルゴリズムやそれを開発する企業、新しいスタイルを収集して学習させるオペレーターは不要です。人間の欲望、無垢な想像力と向き合っていく必要があります。例えば日本語入力IMEのような、予測変換やUI技術を駆使した日本語文字入力技術がなければ、我々日本人はまともに文章を書くことすら難しいでしょう。そのような技術をなくして、小中高校のような鉛筆がなければ書けない日本語のみを「日本語」とすることはとても乱暴で、それすらも「鉛筆やノートという工業製品がなければ成立していません。

そして人工知能で絵を描く以外にも、作文を書くツールなども生まれています。この原稿も、ツールで書いているかもしれません。

//footnote[tweet0824][https://twitter.com/o_ob/status/1562248466490748928]
//footnote[GLIDE][https://github.com/openai/glide-text2im]
//footnote[GLIDE-Paper][GLIDE: Towards Photorealistic Image Generation and Editing with Text-Guided Diffusion Models, https://arxiv.org/abs/2112.10741]
