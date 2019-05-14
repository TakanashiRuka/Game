using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UpdateMat_StageB : MonoBehaviour {
    GameObject gameObject;
    Texture tex2, tex4, tex6, tex8;
    int pageNum, ChangeTex_time, Pagetype;
    bool Backpageflug, Nextpageflug;
    // Use this for initialization
    void Start ()
    {
        pageNum = 0;
        gameObject = GameObject.Find("StageB");
        ChangeTex_time = pageNum = 0;
        Nextpageflug = Backpageflug = false;
        tex2 = Resources.Load<Texture2D>("Texture/Stage2") as Texture2D;
        tex4 = Resources.Load<Texture2D>("Texture/stage4") as Texture2D;
        tex6 = Resources.Load<Texture2D>("Texture/stage6") as Texture2D;
        tex8 = Resources.Load<Texture2D>("Texture/stage8") as Texture2D;

        InputController.SetDefault();
    }
	
	// Update is called once per frame
	void Update ()
    {
        int control;

        InputController.Update();

        control = InputController.GetAxisDown(InputController.Axis.Cross_Horizontal);

        switch (pageNum)
        {
            case 0:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    pageNum += 2;
                    Nextpageflug = true;
                    Pagetype = 3;
                }

                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    pageNum += 1;
                }

                break;
            case 1:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    pageNum += 2;
                    Nextpageflug = true;
                    Pagetype = 3;
                }

                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    pageNum += 1;
                    Nextpageflug = true;
                    Pagetype = 3;
                }

                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    pageNum -= 1;
                }
                break;

            case 2:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    pageNum += 2;
                    Nextpageflug = true;
                    Pagetype = 5;
                }

                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    pageNum += 1;
                }

                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    pageNum -= 1;
                    Backpageflug = true;
                    Pagetype = 1;
                }

                if (Input.GetKeyDown(KeyCode.S))
                {
                    pageNum -= 2;
                    Backpageflug = true;
                    Pagetype = 1;
                }
                break;
            case 3:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    pageNum += 2;
                    Nextpageflug = true;
                    Pagetype = 5;
                }

                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    pageNum += 1;
                    Nextpageflug = true;
                    Pagetype = 5;
                }

                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    pageNum -= 1;

                }

                if (Input.GetKeyDown(KeyCode.S))
                {
                    pageNum -= 2;
                    Backpageflug = true;
                    Pagetype = 1;
                }
                break;

            case 4:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    pageNum += 2;
                    Nextpageflug = true;
                    Pagetype = 7;
                }

                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    pageNum += 1;
                }

                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    pageNum -= 1;
                    Backpageflug = true;
                    Pagetype = 3;
                }

                if (Input.GetKeyDown(KeyCode.S))
                {
                    pageNum -= 2;
                    Backpageflug = true;
                    Pagetype = 3;
                }
                break;
            case 5:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    pageNum += 2;
                    Nextpageflug = true;
                    Pagetype = 7;
                }

                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    pageNum += 1;
                    Nextpageflug = true;
                    Pagetype = 7;
                }

                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    pageNum -= 1;
                }

                if (Input.GetKeyDown(KeyCode.S))
                {
                    pageNum -= 2;
                    Backpageflug = true;
                    Pagetype = 3; ;
                }
                break;
            case 6:

                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    pageNum += 1;
                }

                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    pageNum -= 1;
                    Backpageflug = true;
                    Pagetype = 5;
                }

                if (Input.GetKeyDown(KeyCode.S))
                {
                    pageNum -= 2;
                    Backpageflug = true;
                    Pagetype = 5;
                }
                break;
            case 7:

                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    pageNum -= 1;
                }

                if (Input.GetKeyDown(KeyCode.S))
                {
                    pageNum -= 2;
                    Backpageflug = true;
                    Pagetype = 5;
                }
                break;
        }

        if (Nextpageflug == true)
        {
            ChangeTex_time++;
            if (ChangeTex_time >= 5)
            {
                switch (Pagetype)
                {
                    case 1:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex2;
                        ChangeTex_time = 0;
                        Nextpageflug = false;
                        break;
                    case 3:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex4;
                        ChangeTex_time = 0;
                        Nextpageflug = false;
                        break;
                    case 5:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex6;
                        ChangeTex_time = 0;
                        Nextpageflug = false;
                        break;
                    case 7:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex8;
                        ChangeTex_time = 0;
                        Nextpageflug = false;
                        break;
                }
            }
        }
        if (Backpageflug == true)
        {
            ChangeTex_time++;
            if (ChangeTex_time >= 60)
            {
                switch (Pagetype)
                {
                    case 1:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex2;
                        ChangeTex_time = 0;
                        Backpageflug = false;
                        break;
                    case 3:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex4;
                        ChangeTex_time = 0;
                        Backpageflug = false;
                        break;
                    case 5:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex6;
                        ChangeTex_time = 0;
                        Backpageflug = false;
                        break;
                    case 7:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex8;
                        ChangeTex_time = 0;
                        Backpageflug = false;
                        break;
                }
            }
        }
    }
}
