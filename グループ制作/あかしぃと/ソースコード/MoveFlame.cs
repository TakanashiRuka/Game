using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveFlame : MonoBehaviour {

    public GameObject Flame ;
    Vector3 Flamepos ;
    int clickNum;
    bool LeftLimmit, RightLimmit;
    
    
    // Use this for initialization
    void Start () {
        Flamepos.x  = -3.1f ;
        Flamepos.y  = 0.0f  ;
        Flamepos.z  = 1.2f  ;
        LeftLimmit  = true  ;
        RightLimmit = false ;
        clickNum = 0;
        Flame = GameObject.Find("Plane");

        InputController.SetDefault();
    }

    // Update is called once per frame
    void Update()
    {
        int control;

        InputController.Update();

        control = InputController.GetAxisDown(InputController.Axis.Cross_Horizontal);

        switch (clickNum)
        {
            case 0:

                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    clickNum += 1;
                    RightLimmit = true;
                    LeftLimmit = false;
                    Flamepos.x = 7.0f;
                    Flamepos.z = -3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }
                if (Input.GetKeyDown(KeyCode.W))
                {
                    clickNum += 2;
                }

                break;

            case 1:
                if ((Input.GetKeyDown(KeyCode.A) || control == -1) && (LeftLimmit == true))
                {
                    clickNum -= 1;
                    LeftLimmit = false;
                    RightLimmit = true;
                    Flamepos.x = 7.0f;
                    Flamepos.z = -3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }
                else if ((Input.GetKeyDown(KeyCode.A) || control == -1) && (LeftLimmit == false))
                {
                    clickNum -= 1;
                    RightLimmit = false;
                    LeftLimmit = true;
                    Flamepos.x = -7.0f;
                    Flamepos.z = 3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }

                if ((Input.GetKeyDown(KeyCode.D) || control == 1) && (RightLimmit == true))
                {
                    clickNum += 1;
                    RightLimmit = false;
                    LeftLimmit = true;
                    Flamepos.x = -7.0f;
                    Flamepos.z = 3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);

                }
                else if ((Input.GetKeyDown(KeyCode.D) || control == 1) && (RightLimmit == false))
                {
                    clickNum += 1;
                    LeftLimmit = false;
                    RightLimmit = true;
                    Flamepos.x = 7.0f;
                    Flamepos.z = -3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }
                if (Input.GetKeyDown(KeyCode.W))
                {
                    clickNum += 2;
                }
                break;

            case 6:
                if ((Input.GetKeyDown(KeyCode.A) || control == -1) && (LeftLimmit == true))
                {
                    clickNum -= 1;
                    LeftLimmit = false;
                    RightLimmit = true;
                    Flamepos.x = 7.0f;
                    Flamepos.z = -3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }
                else if ((Input.GetKeyDown(KeyCode.A) || control == -1) && (LeftLimmit == false))
                {
                    clickNum -= 1;
                    RightLimmit = false;
                    LeftLimmit = true;
                    Flamepos.x = -7.0f;
                    Flamepos.z = 3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }

                if ((Input.GetKeyDown(KeyCode.D) || control == 1) && (RightLimmit == true))
                {
                    clickNum += 1;
                    RightLimmit = false;
                    LeftLimmit = true;
                    Flamepos.x = -7.0f;
                    Flamepos.z = 3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);

                }
                else if ((Input.GetKeyDown(KeyCode.D) || control == 1) && (RightLimmit == false))
                {
                    clickNum += 1;
                    LeftLimmit = false;
                    RightLimmit = true;
                    Flamepos.x = 7.0f;
                    Flamepos.z = -3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }
                if (Input.GetKeyDown(KeyCode.S))
                {
                    clickNum -= 2;
                }
                break;
            case 7:
                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    clickNum -= 1;
                    LeftLimmit = true;
                    RightLimmit = false;
                    Flamepos.x = -7.0f;
                    Flamepos.z = 3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }
 
                if (Input.GetKeyDown(KeyCode.S))
                {
                    clickNum -= 2;
                }
                break;

            default:
                if ((Input.GetKeyDown(KeyCode.A) || control == -1) && (LeftLimmit == true))
                {
                    clickNum -= 1;
                    LeftLimmit = false;
                    RightLimmit = true;
                    Flamepos.x = 7.0f;
                    Flamepos.z = -3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }
                else if ((Input.GetKeyDown(KeyCode.A) || control == -1) && (LeftLimmit == false))
                {
                    clickNum -= 1;
                    RightLimmit = false;
                    LeftLimmit = true;
                    Flamepos.x = -7.0f;
                    Flamepos.z = 3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }

                if ((Input.GetKeyDown(KeyCode.D) || control == 1) && (RightLimmit == true))
                {
                    clickNum += 1;
                    RightLimmit = false;
                    LeftLimmit = true;
                    Flamepos.x = -7.0f;
                    Flamepos.z = 3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);

                }
                else if ((Input.GetKeyDown(KeyCode.D) || control == 1) && (RightLimmit == false))
                {
                    clickNum += 1;
                    LeftLimmit = false;
                    RightLimmit = true;
                    Flamepos.x = 7.0f;
                    Flamepos.z = -3.1f;
                    Flamepos.y = 0.0f;
                    transform.Translate(Flamepos);
                }

                if (Input.GetKeyDown(KeyCode.W))
                {
                    clickNum += 2;
                }

                if (Input.GetKeyDown(KeyCode.S))
                {
                    clickNum -= 2;
                }
                break;
        }
            
    }
}
