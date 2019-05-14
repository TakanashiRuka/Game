using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Charcter : MonoBehaviour {

    // public Sprite[] anim;

    //Sprite[] Reon;

    public Sprite[] GreenReon;
    public Sprite[] RedReon;
    public Sprite[] WhiteReon;
    
   

    public Sprite[] NGreenReon;
    public Sprite[] NRedReon;
    public Sprite[] NWhiteReon;
    
    

    Renderer Root;
    //float speed;
    int Animindex;
    int dir;
    bool bWalk;


    //float changeRed = 1.0f;
    //float changeBlue = 0.0f;
    //float changeGreen = 0.0f;
    //float changeAlpha = 1.0f;


    // Rigidbody rigidBody;
    Vector3 Move;
    Vector3 defaultScale = Vector3.zero;



    // Use this for initialization
    void Start()
    {
        //rigidBody = GetComponent<Rigidbody>();

        //リソースフォルダの中のキャラクターフォルダ内の画像を全て配列で取得
      //  Reon = Resources.LoadAll<Sprite>("Texture/Charcter/");

        //Playerタグが付いたオブジェクトの情報取得
        Root = GameObject.FindGameObjectWithTag("Player").GetComponent<Renderer>();

        Animindex = 0;

        dir = 0;

        //speed = 0.05f;

        defaultScale = transform.lossyScale;

        bWalk = false;

        InputController.SetDefault();

    }

    // Update is called once per frame
    void Update()
    {

        

        InputController.Update();

        

        if (bWalk)
        {
            Animindex++;

            if (Animindex >= WhiteReon.Length * 5)
            {
                Animindex = 0;
            }

            //親のカラーによってスプライトを変更
            if (Root.material.color == Color.white)
            {
                GetComponent<SpriteRenderer>().sprite = WhiteReon[Animindex / 5];
            }
            else if (Root.material.color == Color.green)
            {
                GetComponent<SpriteRenderer>().sprite = GreenReon[Animindex / 5];
            }
            else if (Root.material.color == Color.red)
            {
                GetComponent<SpriteRenderer>().sprite = RedReon[Animindex / 5];
            }

        }
        else
        {
            Animindex++;

            if (Animindex >= NWhiteReon.Length * 5)
            {
                Animindex = 0;
            }

            //親のカラーによってスプライトを変更
             if (Root.material.color == Color.white)
            {
                GetComponent<SpriteRenderer>().sprite = NWhiteReon[Animindex / 5];
            }
            else if (Root.material.color == Color.green)
            {
                GetComponent<SpriteRenderer>().sprite = NGreenReon[Animindex / 5];
            }
            else if (Root.material.color == Color.red)
            {
                GetComponent<SpriteRenderer>().sprite = NRedReon[Animindex / 5];
            }
        }

        /* if(Input.GetKeyDown(KeyCode.S))
         {
             this.GetComponent<SpriteRenderer>().color = new Color(changeRed,changeGreen,changeBlue,changeAlpha);//スプライトのカラー変更
         }*/

        

        if (Move.x != 0)
        {
            bWalk = true;
        }
        else if (bWalk)
        {
            bWalk = false;
        }

        if (Move.x < 0)
        {
            dir = -1;
        }
        else if (Move.x > 0)
        {
            dir = 1;
        }
    }

    void FixedUpdate()
    {
        //親のスケールに影響されないスケール設定
        Vector2 lossScale = transform.lossyScale; //実際のスケールを登録
        Vector2 localScale = transform.localScale;

        float control;

        control = InputController.GetAxis(InputController.Axis.Cross_Horizontal);

        Move.x = Input.GetAxis("Horizontal");

        if (control != 0)
        {
            Move.x = control;
        }


        //transform.position += Move * speed;

        transform.localScale = new Vector2(Mathf.Sign(dir) * localScale.x / lossScale.x * defaultScale.x,
                                          localScale.y / lossScale.y * defaultScale.y );

    }

    public bool GetWalk()
    {
        return bWalk;
    }
}
