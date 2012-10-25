<html>
    <head>
    </head>
    <body style="width:1000px;">
        <div style="width:400px;">
            <form name="TCC" method="get">
                <div>
                    <span>usuario: </span>
                    <input type='text' value='' name='usuario'/>
                </div>
                <div>
                    <span>Nome:</span>
                    <input type='text' value='' name='nome'/>
                </div>
                <div>
                    <span>Senha: </span>
                    <input type='password' value='' name='senha'/>
                </div>
                <input type="submit" name="send" value="Enviar"/>
            </form>
            <div>
                <?php
                ini_set("display_errors", 1);
                $link = mysql_connect('localhost', 'root', 'root');
               // if (!$link) {
                    echo "Erro no link com DB" .
                    die();
               // }
                $db_selected = mysql_select_db('TCC_TESTE', $link);
               // if (!$db_selected) {
                    echo "DB_NAO SELECIONADO";
                    die();
               // }
              //  if (isset($_GET['usuario'])) {
                    $nome = htmlspecialchars($_GET['nome']);
                    $usuario = htmlspecialchars($_GET['usuario']);
                    $query = "INSERT INTO MERDA(usuario, nome) VALUES ('$usuario', '$nome')";
                    $result = mysql_query($query);
               // }

                $sql = "SELECT nome as nome, usuario as usuario from MERDA";
                $result = mysql_query($sql);
                //echo mysql_errno($link) . ": " . mysql_error($link). "\n";
               // if ($result) {
               //     while ($col = mysql_fetch_array($result)) {
                        echo $col[0] . " " . $col[1] . "<BR/>";
              //      }
             //   }
                // $col[0] = "Evandro PHODASTICO";
                // $cabelo['ponta'] = "duplos $col[0]";
                // echo "quem tem $col[0]";
                ?>
            </div>
        </div>
        <div style="width:400px;">
            <?php
            /*if(isset($_GET['pesquisa']){
                $pesquisa = $_GET['pesquisa'];
                $sql = "SELECT * FROM MERDA WHERE id = '$pesquisa'";
                $result = mysql_query ($sql);
                if ($result){
                    
                    
                }
            }*/
            
            ?>

        </div>
    </body>
</html>
