using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UpdateMat_StageA : MonoBehaviour {
    public
    GameObject gameObject;
    Material mat;
    Texture2D tex1, tex3, tex5, tex7;
    int pageNum,ChangeTex_time,Pagetype;
    bool Backpageflug,Nextpageflug;
	// Use this for initialization
	void Start ()
    {
        gameObject = GameObject.Find("StageA");
        ChangeTex_time = pageNum = 0;
        Nextpageflug = Backpageflug = false;
        mat = GetComponent<Renderer>().material;
        tex1 = Resources.Load<Texture2D>("Texture/Stage1")as Texture2D;
        tex3 = Resources.Load<Texture2D>("Texture/stage3")as Texture2D;
        tex5 = Resources.Load<Texture2D>("Texture/stage5")as Texture2D;
        tex7 = Resources.Load<Texture2D>("Texture/stage7")as Texture2D;

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
                    Pagetype = 3;;
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
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex1;
                        ChangeTex_time = 0;
                        Nextpageflug = false;
                        break;
                    case 3:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex3;
                        ChangeTex_time = 0;
                        Nextpageflug = false;
                        break;
                    case 5:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex5;
                        ChangeTex_time = 0;
                        Nextpageflug = false;
                        break;
                    case 7:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex7;
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
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex1;
                        ChangeTex_time = 0;
                        Backpageflug = false;
                        break;
                    case 3:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex3;
                        ChangeTex_time = 0;
                        Backpageflug = false;
                        break;
                    case 5:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex5;
                        ChangeTex_time = 0;
                        Backpageflug = false;
                        break;
                    case 7:
                        gameObject.GetComponent<Renderer>().material.mainTexture = tex7;
                        ChangeTex_time = 0;
                        Backpageflug = false;
                        break;
                }
            }
        }

    }
}
