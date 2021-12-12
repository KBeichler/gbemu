#include <cpu.h>

uint8_t OpCodeTimingTable[0x100] = {
1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1,
1, 3, 2, 2, 1, 1, 2, 1, 0, 2, 2, 2, 1, 1, 2, 1,
2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1,
2, 3, 2, 2, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 2, 1,
1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1,
1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
2, 3, 3, 4, 3, 4, 2, 4, 2, 1, 3, 1, 3, 6, 2, 4,
2, 3, 3, 0, 3, 4, 2, 4, 2, 1, 3, 0, 3, 0, 2, 4,
3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4,
3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4,
};
uint8_t IRQ_TABLE[5] = { 0x40, 0x48, 0x50, 0x58, 0x60};
cpu_t cpu;


extern mem_t mem;



void cpu_init(){
    memset((void *) &cpu, 0, sizeof(cpu_t));
    cpu.prefixCode = 0;
    
    cpu.irq_enable = 1;

}













void cpu_tick()
{


    // check interrupts
    if (cpu.irq_enable == 1)
    {
        for (uint8_t i = 0; i < 5 && cpu.irq_enable; i++)
        {
            if ( ( mem.IE & (1 << i)) && ( mem.IF & (1 << i)) )
            {

                PUSH(REG(PC));
                REG(PC) = IRQ_TABLE[i];
                //clear flag
                mem.IF &= ~( 1 << i );
                DISABLE_IRQ;
            }
        }


    }




    uint8_t code = mem_read(REG(PC)++);
    // if current opcode is not a prefixed opcode
    if (cpu.prefixCode == 0)
    {
        cpu.currentCycleLength = OpCodeTimingTable[code];
        switch (code)
        {
            case    0x00:   {                                               }; break;
            case	0x01:	{ LD_RR_D16(REG(BC));                           }; break; 	                    
            case	0x02:	{ LD_MEM_n( REG(BC), REG(A) );				    }; break;
            case	0x03:	{ INC_RR(REG(BC));				                }; break;
            case	0x04:	{ INC_R(REG(B));				                }; break;
            case	0x05:	{ DEC_R(REG(B));                                }; break;
            case	0x06:	{ LD_R_n(REG(B), mem_read(REG(PC++)));   	    }; break;
            case	0x07:	{ RLC(REG(A)); FLAG(Z) = 0;                     }; break;
            case	0x08:	{ uint16_t a = mem_read(cpu.PC++);
                              a |=  mem_read(cpu.PC++) << 8;
                              mem_write( a , cpu.SP & 0xFF);
                              mem_write( a+1 , cpu.SP >> 8);                }; break;
            case	0x09:	{ ADD_RR(REG(BC));				                }; break;
            case	0x0A:	{ LD_R_n(REG(A), mem_read( REG(BC) ));			}; break;
            case	0x0B:	{ DEC_RR(REG(BC));			                    }; break;
            case	0x0C:	{ INC_R(REG(C));	 			                }; break;
            case	0x0D:	{ DEC_R(REG(C));				                }; break;
            case	0x0E:	{ LD_R_n(REG(C), mem_read(REG(PC++)));			}; break;
            case	0x0F:	{ RRC(REG(A)); FLAG(Z) = 0;                     }; break;
            case	0x10:	{        	                                    }; break; // STOP
            case	0x11:	{ LD_RR_D16(REG(DE));				            }; break;
            case	0x12:	{ LD_MEM_n( REG(DE), REG(A) );		    	    }; break;
            case	0x13:	{ INC_RR(REG(DE));							    }; break;
            case	0x14:	{ INC_R(REG(D));							    }; break;
            case	0x15:	{ DEC_R(REG(D));                                }; break;
            case	0x16:	{ LD_R_n(REG(D), mem_read(REG(PC++)));  		}; break;
            case	0x17:	{ RL(REG(A)); FLAG(Z) = 0;                      }; break;
            case	0x18:	{ JMP_n(1);                         		    }; break;
            case	0x19:	{ ADD_RR(REG(DE));					            }; break;
            case	0x1A:	{ LD_R_n(REG(A), mem_read(REG(DE)));			}; break;
            case	0x1B:	{ DEC_RR(REG(DE));				                }; break;
            case	0x1C:	{ INC_R(REG(E));	 							}; break;
            case	0x1D:	{ DEC_R(REG(E));								}; break;
            case	0x1E:	{ LD_R_n(REG(E), mem_read(REG(PC++)));		    }; break;
            case	0x1F:	{ RR(REG(A)); FLAG(Z) = 0;                      }; break;
            case	0x20:	{ JMP_n( FLAG(Z) == 0);                         }; break;

            case	0x21:	{ LD_RR_D16(REG(HL));					        }; break;
            case	0x22:	{ LD_MEM_n( REG(HL++), REG(A) );				}; break;
            case	0x23:	{ INC_RR(REG(HL));								}; break;
            case	0x24:	{ INC_R(REG(H));								}; break;
            case	0x25:	{ DEC_R(REG(H));                                }; break;
            case	0x26:	{ LD_R_n(REG(H), mem_read(REG(PC++)));  		}; break;

            // DAA
            case	0x27:	{ //change Register A from dec to BCD, check on flags to set correctly!
                                                                            }; break;

            case	0x28:	{ JMP_n( FLAG(Z));                              }; break;
            case	0x29:	{ ADD_RR(REG(HL));					            }; break;
            case	0x2A:	{ LD_R_n(REG(A), mem_read(REG(HL++)));			}; break;
            case	0x2B:	{ DEC_RR(REG(HL));				                }; break;
            case	0x2C:	{ INC_R(REG(L));	 							}; break;
            case	0x2D:	{ DEC_R(REG(L));								}; break;
            case	0x2E:	{ LD_R_n(REG(L), mem_read(REG(PC++)));			}; break;
            case	0x2F:	{ FLAG(N) = FLAG(HC) = 1; REG(A) ^= 0xFF;	    }; break;
            case	0x30:	{ JMP_n( FLAG(CY) == 0);                        }; break;
            case	0x31:	{ LD_RR_D16(REG(SP));					        }; break;
            case	0x32:	{ LD_MEM_n( REG(HL--), REG(A) );			    }; break;
            case	0x33:	{ INC_RR(REG(SP));								}; break;
            case	0x34:	{ INC_HL;                                       }; break;
            case	0x35:	{ DEC_HL;                                       }; break;
            case	0x36:	{ LD_MEM_n( REG(HL), mem_read(REG(PC++)));  	}; break;
            case	0x37:	{ FLAG(HC) = FLAG(N) = 0; FLAG(CY) = 1;			}; break;
            case	0x38:	{ JMP_n( FLAG(CY) );                            }; break;
            case	0x39:	{ ADD_RR(REG(SP));					            }; break;
            case	0x3A:	{ LD_R_n(REG(A), mem_read(REG(HL--)));			}; break;
            case	0x3B:	{ DEC_RR(REG(SP));			                    }; break;
            case	0x3C:	{ INC_R(REG(A));	 							}; break;
            case	0x3D:	{ DEC_R(REG(A));								}; break;
            case	0x3E:	{ LD_R_n(REG(A), mem_read(REG(PC++)));			}; break;
            case	0x3F:	{ FLAG(HC) = FLAG(N) = 0; FLAG(CY) ^= 1;		}; break;

            // REGISTER LOADS
            case	0x40:	{ LD_R_n(REG(B) , REG(B)  );				    }; break;
            case	0x41:	{ LD_R_n(REG(B) , REG(C)  );				    }; break;
            case	0x42:	{ LD_R_n(REG(B) , REG(D)  );				    }; break;
            case	0x43:	{ LD_R_n(REG(B) , REG(E)  );				    }; break;
            case	0x44:	{ LD_R_n(REG(B) , REG(H)  );				    }; break;
            case	0x45:	{ LD_R_n(REG(B) , REG(L)  );				    }; break;
            case	0x46:	{ LD_R_n(REG(B) , mem_read( REG(HL)) );		    }; break;
            case	0x47:	{ LD_R_n(REG(B) , REG(A)  );				    }; break;
            case	0x48:	{ LD_R_n(REG(C) , REG(B)  );				    }; break;
            case	0x49:	{ LD_R_n(REG(C) , REG(C)  );				    }; break;
            case	0x4A:	{ LD_R_n(REG(C) , REG(D)  );				    }; break;
            case	0x4B:	{ LD_R_n(REG(C) , REG(E)  );				    }; break;
            case	0x4C:	{ LD_R_n(REG(C) , REG(H)  );				    }; break;
            case	0x4D:	{ LD_R_n(REG(C) , REG(L)  );				    }; break;
            case	0x4E:	{ LD_R_n(REG(C) , mem_read( REG(HL)) );         }; break;
            case	0x4F:	{ LD_R_n(REG(C) , REG(A)  );				    }; break;
            case	0x50:	{ LD_R_n(REG(D) , REG(B)  );				    }; break;
            case	0x51:	{ LD_R_n(REG(D) , REG(C)  );				    }; break;
            case	0x52:	{ LD_R_n(REG(D) , REG(D)  );				    }; break;
            case	0x53:	{ LD_R_n(REG(D) , REG(E)  );				    }; break;
            case	0x54:	{ LD_R_n(REG(D) , REG(H)  );				    }; break;
            case	0x55:	{ LD_R_n(REG(D) , REG(L)  );				    }; break;
            case	0x56:	{ LD_R_n(REG(D) , mem_read( REG(HL)) );         }; break;
            case	0x57:	{ LD_R_n(REG(D) , REG(A)  );				    }; break;
            case	0x58:	{ LD_R_n(REG(E) , REG(B)  );				    }; break;
            case	0x59:	{ LD_R_n(REG(E) , REG(C)  );				    }; break;
            case	0x5A:	{ LD_R_n(REG(E) , REG(D)  );				    }; break;
            case	0x5B:	{ LD_R_n(REG(E) , REG(E)  );				    }; break;
            case	0x5C:	{ LD_R_n(REG(E) , REG(H)  );				    }; break;
            case	0x5D:	{ LD_R_n(REG(E) , REG(L)  );				    }; break;
            case	0x5E:	{ LD_R_n(REG(E) , mem_read( REG(HL)) );	        }; break;
            case	0x5F:	{ LD_R_n(REG(E) , REG(A)  );				    }; break;
            case	0x60:	{ LD_R_n(REG(H) , REG(B)  );				    }; break;
            case	0x61:	{ LD_R_n(REG(H) , REG(C)  );				    }; break;
            case	0x62:	{ LD_R_n(REG(H) , REG(D)  );				    }; break;
            case	0x63:	{ LD_R_n(REG(H) , REG(E)  );				    }; break;
            case	0x64:	{ LD_R_n(REG(H) , REG(H)  );				    }; break;
            case	0x65:	{ LD_R_n(REG(H) , REG(L)  );				    }; break;
            case	0x66:	{ LD_R_n(REG(H) , mem_read( REG(HL)) );	        }; break;
            case	0x67:	{ LD_R_n(REG(H) , REG(A)  );				    }; break;
            case	0x68:	{ LD_R_n(REG(L) , REG(B)  );				    }; break;
            case	0x69:	{ LD_R_n(REG(L) , REG(C)  );				    }; break;
            case	0x6A:	{ LD_R_n(REG(L) , REG(D)  );				    }; break;
            case	0x6B:	{ LD_R_n(REG(L) , REG(E)  );				    }; break;
            case	0x6C:	{ LD_R_n(REG(L) , REG(H)  );				    }; break;
            case	0x6D:	{ LD_R_n(REG(L) , REG(L)  );				    }; break;
            case	0x6E:	{ LD_R_n(REG(L) , mem_read( REG(HL)) );	        }; break;
            case	0x6F:	{ LD_R_n(REG(L) , REG(A)  );				    }; break;
            case	0x70:	{ LD_MEM_n( REG(HL), REG(B) );	                }; break;
            case	0x71:	{ LD_MEM_n( REG(HL), REG(C) );	                }; break;
            case	0x72:	{ LD_MEM_n( REG(HL), REG(D) );	                }; break;
            case	0x73:	{ LD_MEM_n( REG(HL), REG(E) );	                }; break;
            case	0x74:	{ LD_MEM_n( REG(HL), REG(H) );	                }; break;
            case	0x75:	{ LD_MEM_n( REG(HL), REG(L) );	                }; break;
            case	0x76:	{   /* TODO HALT*/                              }; break; // HALT
            case	0x77:	{ LD_MEM_n(REG(HL) , REG(A)  );			        }; break;
            case	0x78:	{ LD_R_n(REG(A) , REG(B)  );				    }; break;
            case	0x79:	{ LD_R_n(REG(A) , REG(C)  );				    }; break;
            case	0x7A:	{ LD_R_n(REG(A) , REG(D)  );				    }; break;
            case	0x7B:	{ LD_R_n(REG(A) , REG(E)  );				    }; break;
            case	0x7C:	{ LD_R_n(REG(A) , REG(H)  );				    }; break;
            case	0x7D:	{ LD_R_n(REG(A) , REG(L)  );				    }; break;
            case	0x7E:	{ LD_R_n(REG(A) , mem_read(REG(HL)));			}; break;
            case	0x7F:	{ LD_R_n(REG(A) , REG(A)  );				    }; break;

            // ARTIHMETIC 
            case	0x80:	{ ADD_R(REG(B)  );                              }; break;
            case	0x81:	{ ADD_R(REG(C)  );                              }; break;
            case	0x82:	{ ADD_R(REG(D)  );                              }; break;
            case	0x83:	{ ADD_R(REG(E)  );                              }; break;
            case	0x84:	{ ADD_R(REG(H)  );                              }; break;
            case	0x85:	{ ADD_R(REG(L)  );                              }; break;
            case	0x86:	{ ADD_R(mem_read(REG(HL)));                     }; break;
            case	0x87:	{ ADD_R(REG(A)  );                              }; break;
            case	0x88:	{ ADC_R(REG(B)  );                              }; break;
            case	0x89:	{ ADC_R(REG(C)  );                              }; break;
            case	0x8A:	{ ADC_R(REG(D)  );                              }; break;
            case	0x8B:	{ ADC_R(REG(E)  );                              }; break;
            case	0x8C:	{ ADC_R(REG(H)  );                              }; break;
            case	0x8D:	{ ADC_R(REG(L)  );                              }; break;
            case	0x8E:	{ ADC_R(mem_read(REG(HL)));                     }; break;
            case	0x8F:	{ ADC_R(REG(A)  );                              }; break;
            case	0x90:	{ SUB_R(REG(B)  );                              }; break;
            case	0x91:	{ SUB_R(REG(C)  );                              }; break;
            case	0x92:	{ SUB_R(REG(D)  );                              }; break;
            case	0x93:	{ SUB_R(REG(E)  );                              }; break;
            case	0x94:	{ SUB_R(REG(H)  );                              }; break;
            case	0x95:	{ SUB_R(REG(L)  );                              }; break;
            case	0x96:	{ SUB_R(mem_read(REG(HL)));                     }; break;
            case	0x97:	{ SUB_R(REG(A)  );                              }; break;
            case	0x98:	{ SBC_R(REG(B)  );                              }; break;
            case	0x99:	{ SBC_R(REG(C)  );                              }; break;
            case	0x9A:	{ SBC_R(REG(D)  );                              }; break;
            case	0x9B:	{ SBC_R(REG(E)  );                              }; break;
            case	0x9C:	{ SBC_R(REG(H)  );                              }; break;
            case	0x9D:	{ SBC_R(REG(L)  );                              }; break;
            case	0x9E:	{ SBC_R(mem_read(REG(HL)));                     }; break;
            case	0x9F:	{ SBC_R(REG(A)  );                              }; break;
            case	0xA0:	{ AND_R(REG(B)  );                              }; break;
            case	0xA1:	{ AND_R(REG(C)  );                              }; break;
            case	0xA2:	{ AND_R(REG(D)  );                              }; break;
            case	0xA3:	{ AND_R(REG(E)  );                              }; break;
            case	0xA4:	{ AND_R(REG(H)  );                              }; break;
            case	0xA5:	{ AND_R(REG(L)  );                              }; break;
            case	0xA6:	{ AND_R(mem_read(REG(HL)));                     }; break;
            case	0xA7:	{ AND_R(REG(A)  );                              }; break;
            case	0xA8:	{ XOR_R(REG(B)  );                              }; break;
            case	0xA9:	{ XOR_R(REG(C)  );                              }; break;
            case	0xAA:	{ XOR_R(REG(D)  );                              }; break;
            case	0xAB:	{ XOR_R(REG(E)  );                              }; break;
            case	0xAC:	{ XOR_R(REG(H)  );                              }; break;
            case	0xAD:	{ XOR_R(REG(L)  );                              }; break;
            case	0xAE:	{ XOR_R(mem_read(REG(HL)));                     }; break;
            case	0xAF:	{ XOR_R(REG(A)  );                              }; break;
            case	0xB0:	{  OR_R(REG(B)  );                              }; break;
            case	0xB1:	{  OR_R(REG(C)  );                              }; break;
            case	0xB2:	{  OR_R(REG(D)  );                              }; break;
            case	0xB3:	{  OR_R(REG(E)  );                              }; break;
            case	0xB4:	{  OR_R(REG(H)  );                              }; break;
            case	0xB5:	{  OR_R(REG(L)  );                              }; break;
            case	0xB6:	{  OR_R(mem_read(REG(HL)));                     }; break;
            case	0xB7:	{  OR_R(REG(A)  );                              }; break;
            case	0xB8:	{  CP_R(REG(B)  );                              }; break;
            case	0xB9:	{  CP_R(REG(C)  );                              }; break;
            case	0xBA:	{  CP_R(REG(D)  );                              }; break;
            case	0xBB:	{  CP_R(REG(E)  );                              }; break;
            case	0xBC:	{  CP_R(REG(H)  );                              }; break;
            case	0xBD:	{  CP_R(REG(L)  );                              }; break;
            case	0xBE:	{  CP_R(mem_read(REG(HL)));                     }; break;
            case	0xBF:	{  CP_R(REG(A)  );                              }; break;


            case	0xC0:	{ RET( FLAG(Z) == 0);				            }; break;
            case	0xC1:	{ POP( REG(BC) );				                }; break;
            case	0xC2:	{ JMP_nn( FLAG(Z) == 0 );      	    			}; break;
            case	0xC3:	{ JMP_nn (1);				                    }; break;
            case	0xC4:	{ CALL( FLAG(Z) == 0 );		                    }; break;
            case	0xC5:	{ PUSH(REG(BC));				                }; break;
            case	0xC6:	{ ADD_R(mem_read(REG(PC++)));                   }; break;
            case	0xC7:	{ RST(0x00);				                    }; break;
            case	0xC8:	{ RET( FLAG(Z) );            				    }; break;
            case	0xC9:	{ RET(1)        		                        }; break;
            case	0xCA:	{ JMP_nn( FLAG(Z) );  				            }; break;
            case	0xCB:	{ cpu.prefixCode = 1;                           }; break;
            case	0xCC:	{ CALL( FLAG(Z) );				                }; break;
            case	0xCD:	{ CALL(1);				                        }; break;
            case	0xCE:	{ ADC_R(mem_read(REG(PC++)));				    }; break;
            case	0xCF:	{ RST(0x08);			                        }; break;
            case	0xD0:	{ RET( FLAG(CY) == 0)	            			}; break;
            case	0xD1:	{ POP(REG(DE));				                    }; break;
            case	0xD2:	{ JMP_nn( FLAG(CY) == 0);     				    }; break;
            case	0xD3:	{               			                    }; break; // INVALID
            case	0xD4:	{ CALL( FLAG(CY) == 0 )	;       			    }; break;
            case	0xD5:	{ PUSH(REG(DE));				                }; break;
            case	0xD6:	{ SUB_R(mem_read(REG(PC++)));				    }; break;
            case	0xD7:	{ RST(0x10);				                    }; break;
            case	0xD8:	{ RET( FLAG(CY) );         				        }; break;
            case	0xD9:	{ RET(1); ENABLE_IRQ;				            }; break;
            case	0xDA:	{ JMP_nn( FLAG(CY) );   			            }; break;
            case	0xDB:	{               				                }; break; // INVALID
            case	0xDC:	{ CALL( FLAG(CY) );			                    }; break;
            case	0xDD:	{               				                }; break; // INVALID
            case	0xDE:	{ SBC_R(mem_read(REG(PC++)));				    }; break;
            case	0xDF:	{ RST(0x18);				                    }; break;
            case	0xE0:	{ LD_MEM_n( 0xFF00 + mem_read(REG(PC++)), REG(A));}; break;
            case	0xE1:	{ POP(REG(HL));					                }; break;
            case	0xE2:	{ LD_MEM_n( (0xFF00 + REG(C)) , REG(A));		}; break;
            case	0xE3:	{             				                    }; break; // INVALID
            case	0xE4:	{               			                    }; break; // INVALID
            case	0xE5:	{ PUSH(REG(HL));			                    }; break;
            case	0xE6:	{ AND_R(mem_read(REG(PC++))) ;	                }; break;
            case	0xE7:	{ RST(0x20)				                        }; break;
            case	0xE8:	{ ADD_SP; 			                            }; break;
            case	0xE9:	{ REG(PC) = REG(HL);				            }; break;
            case	0xEA:	{ uint16_t adr = mem_read(REG(PC++));
                              adr |= mem_read(REG(PC++)) << 8;
                              mem_write(adr, REG(A));                         }; break;
            case	0xEB:	{                   			                }; break; // INVALID
            case	0xEC:	{               				                }; break; // INVALID
            case	0xED:	{               				                }; break; // INVALID
            case	0xEE:	{ XOR_R(mem_read(REG(PC++)));				        }; break;
            case	0xEF:	{ RST(0x28)				                        }; break;
            case	0xF0:	{ LD_R_n( REG(A) , mem_read( 0xFF00 + mem_read(REG(PC++))) );}; break;
            case	0xF1:	{ POP(REG(AF)); REG(F) &= 0xF0;					}; break;
            case	0xF2:	{ LD_R_n( REG(A), mem_read( 0xFF00 + REG(C)) );		}; break;
            case	0xF3:	{ DISABLE_IRQ;				                    }; break;
            case	0xF4:	{                 				                }; break;
            case	0xF5:	{ PUSH(REG(AF));			                    }; break;
            case	0xF6:	{ OR_R(mem_read(REG(PC++)));			                }; break;
            case	0xF7:	{ RST(0x30)				                        }; break;
            case	0xF8:	{ LDHL;                                         }; break;
            case	0xF9:	{ LD_R_n( REG(SP), REG(HL) );				        }; break;
            case	0xFA:	{ uint16_t adr = mem_read(REG(PC++));
                            adr |= mem_read(REG(PC++)) << 8;
                            REG(A) = mem_read(adr);                      }; break;
            case	0xFB:	{ ENABLE_IRQ;				                    }; break;
            case	0xFC:	{               			                    }; break; // INVALID
            case	0xFD:	{               			                    }; break; // INVALID
            case	0xFE:	{ CP_R(mem_read(REG(PC++)));                    }; break;
            case	0xFF:	{ RST(0x38)				                        }; break;
            
            default:                      break;
        
        }
        // end if SWITCH
    }

    // it is is a prefixed opcode
    else
    {
        // 0x-7 and 0x-E ecces memory and take 4 cylces
        cpu.currentCycleLength = (code % 7 == 0 && code != 0) ? 4 : 2;
        cpu.prefixCode = 0;
        switch (code)
        {
            case	0x00:	{ RLC(REG(B));				                }; break;
            case	0x01:	{ RLC(REG(C));				                }; break;
            case	0x02:	{ RLC(REG(D));				                }; break;
            case	0x03:	{ RLC(REG(E));				                }; break;
            case	0x04:	{ RLC(REG(H));				                }; break;
            case	0x05:	{ RLC(REG(L));				                }; break;
            case	0x06:	{ uint8_t n = mem_read(REG(HL));
                                RLC(n);
                                mem_write(REG(HL), n);	                }; break;
            case	0x07:	{ RLC(REG(A));				                }; break;
            case	0x08:	{ RRC(REG(B));								}; break;
            case	0x09:	{ RRC(REG(C));								}; break;
            case	0x0A:	{ RRC(REG(D));								}; break;
            case	0x0B:	{ RRC(REG(E));								}; break;
            case	0x0C:	{ RRC(REG(H));								}; break;
            case	0x0D:	{ RRC(REG(L));								}; break;
            case	0x0E:	{ uint8_t n = mem_read(REG(HL));
                                RRC(n);
                                mem_write(REG(HL), n);						}; break;
            case	0x0F:	{ RRC(REG(A));								}; break;
            case	0x10:	{ RL(REG(B));								}; break;
            case	0x11:	{ RL(REG(C));								}; break;
            case	0x12:	{ RL(REG(D));								}; break;
            case	0x13:	{ RL(REG(E));								}; break;
            case	0x14:	{ RL(REG(H));								}; break;
            case	0x15:	{ RL(REG(L));								}; break;
            case	0x16:	{ uint8_t n = mem_read(REG(HL));
                                RL(n);
                                mem_write(REG(HL), n); 						}; break;
            case	0x17:	{ RL(REG(A));								}; break;
            case	0x18:	{ RR(REG(B));								}; break;
            case	0x19:	{ RR(REG(C));								}; break;
            case	0x1A:	{ RR(REG(D));								}; break;
            case	0x1B:	{ RR(REG(E));								}; break;
            case	0x1C:	{ RR(REG(H));								}; break;
            case	0x1D:	{ RR(REG(L));								}; break;
            case	0x1E:	{ uint8_t n = mem_read(REG(HL));
                                RR(n);
                                mem_write(REG(HL), n); 						}; break;
            case	0x1F:	{ SLA(REG(A));								}; break;
            case	0x20:	{ SLA(REG(B));								}; break;
            case	0x21:	{ SLA(REG(C));								}; break;
            case	0x22:	{ SLA(REG(D));								}; break;
            case	0x23:	{ SLA(REG(E));								}; break;
            case	0x24:	{ SLA(REG(H));								}; break;
            case	0x25:	{ SLA(REG(L));								}; break;
            case	0x26:	{ uint8_t n = mem_read(REG(HL));
                                SLA(n);
                                mem_write(REG(HL), n); 						}; break;
            case	0x27:	{ SRA(REG(A));								}; break;
            case	0x28:	{ SRA(REG(B));								}; break;
            case	0x29:	{ SRA(REG(C));								}; break;
            case	0x2A:	{ SRA(REG(D));								}; break;
            case	0x2B:	{ SRA(REG(E));								}; break;
            case	0x2C:	{ SRA(REG(H));								}; break;
            case	0x2D:	{ SRA(REG(L));								}; break;
            case	0x2E:	{ uint8_t n = mem_read(REG(HL));
                                SRA(n);
                                mem_write(REG(HL), n);						}; break;
            case	0x2F:	{ SRA(REG(A));								}; break;
            case	0x30:	{ SWAP(REG(B));								}; break;
            case	0x31:	{ SWAP(REG(C));								}; break;
            case	0x32:	{ SWAP(REG(D));								}; break;
            case	0x33:	{ SWAP(REG(E));								}; break;
            case	0x34:	{ SWAP(REG(H));								}; break;
            case	0x35:	{ SWAP(REG(L));								}; break;
            case	0x36:	{ uint8_t n = mem_read(REG(HL));
                                SWAP(n);
                                mem_write(REG(HL), n); 						}; break;
            case	0x37:	{ SWAP(REG(A));								}; break;
            case	0x38:	{ SRL(REG(B));								}; break;
            case	0x39:	{ SRL(REG(C));								}; break;
            case	0x3A:	{ SRL(REG(D));								}; break;
            case	0x3B:	{ SRL(REG(E));								}; break;
            case	0x3C:	{ SRL(REG(H));								}; break;
            case	0x3D:	{ SRL(REG(L));								}; break;
            case	0x3E:	{ uint8_t n = mem_read(REG(HL));
                                SRL(n);
                                mem_write(REG(HL), n); 						}; break;
            case	0x3F:	{ SRL(REG(A));								}; break;
            case	0x40:	{ BIT(ARG_1(code) , REG(B));                }; break;
            case	0x41:	{ BIT(ARG_1(code) , REG(C));                }; break;
            case	0x42:	{ BIT(ARG_1(code) , REG(D));                }; break;
            case	0x43:	{ BIT(ARG_1(code) , REG(E));                }; break;
            case	0x44:	{ BIT(ARG_1(code) , REG(H));                }; break;
            case	0x45:	{ BIT(ARG_1(code) , REG(L));                }; break;
            case	0x46:	{ uint8_t n = mem_read(REG(HL));
                                BIT(ARG_1(code) , n); 
                                mem_write(REG(HL), n);		        	}; break;
            case	0x47:	{ BIT(ARG_1(code) , REG(A));				}; break;
            case	0x48:	{ BIT(ARG_1(code) , REG(B));                }; break;
            case	0x49:	{ BIT(ARG_1(code) , REG(C));                }; break;
            case	0x4A:	{ BIT(ARG_1(code) , REG(D));                }; break;
            case	0x4B:	{ BIT(ARG_1(code) , REG(E));                }; break;
            case	0x4C:	{ BIT(ARG_1(code) , REG(H));                }; break;
            case	0x4D:	{ BIT(ARG_1(code) , REG(L));                }; break;
            case	0x4E:	{ uint8_t n = mem_read(REG(HL));
                                BIT(ARG_1(code) , n); 
                                mem_write(REG(HL), n);   			}; break;
            case	0x4F:	{ BIT(ARG_1(code) , REG(A));				}; break;
            case	0x50:	{ BIT(ARG_1(code) , REG(B));                }; break;
            case	0x51:	{ BIT(ARG_1(code) , REG(C));                }; break;
            case	0x52:	{ BIT(ARG_1(code) , REG(D));                }; break;
            case	0x53:	{ BIT(ARG_1(code) , REG(E));                }; break;
            case	0x54:	{ BIT(ARG_1(code) , REG(H));                }; break;
            case	0x55:	{ BIT(ARG_1(code) , REG(L));                }; break;
            case	0x56:	{ uint8_t n = mem_read(REG(HL));
                                BIT(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x57:	{ BIT(ARG_1(code) , REG(A));				}; break;
            case	0x58:	{ BIT(ARG_1(code) , REG(B));                }; break;
            case	0x59:	{ BIT(ARG_1(code) , REG(C));                }; break;
            case	0x5A:	{ BIT(ARG_1(code) , REG(D));                }; break;
            case	0x5B:	{ BIT(ARG_1(code) , REG(E));                }; break;
            case	0x5C:	{ BIT(ARG_1(code) , REG(H));                }; break;
            case	0x5D:	{ BIT(ARG_1(code) , REG(L));                }; break;
            case	0x5E:	{ uint8_t n = mem_read(REG(HL));
                                BIT(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x5F:	{ BIT(ARG_1(code) , REG(A));				}; break;
            case	0x60:	{ BIT(ARG_1(code) , REG(B));                }; break;
            case	0x61:	{ BIT(ARG_1(code) , REG(C));                }; break;
            case	0x62:	{ BIT(ARG_1(code) , REG(D));                }; break;
            case	0x63:	{ BIT(ARG_1(code) , REG(E));                }; break;
            case	0x64:	{ BIT(ARG_1(code) , REG(H));                }; break;
            case	0x65:	{ BIT(ARG_1(code) , REG(L));                }; break;
            case	0x66:	{ uint8_t n = mem_read(REG(HL));
                                BIT(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x67:	{ BIT(ARG_1(code) , REG(A));				}; break;
            case	0x68:	{ BIT(ARG_1(code) , REG(B));                }; break;
            case	0x69:	{ BIT(ARG_1(code) , REG(C));                }; break;
            case	0x6A:	{ BIT(ARG_1(code) , REG(D));                }; break;
            case	0x6B:	{ BIT(ARG_1(code) , REG(E));                }; break;
            case	0x6C:	{ BIT(ARG_1(code) , REG(H));                }; break;
            case	0x6D:	{ BIT(ARG_1(code) , REG(L));                }; break;
            case	0x6E:	{ uint8_t n = mem_read(REG(HL));
                                BIT(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x6F:	{ BIT(ARG_1(code) , REG(A));				}; break;
            case	0x70:	{ BIT(ARG_1(code) , REG(B));                }; break;
            case	0x71:	{ BIT(ARG_1(code) , REG(C));                }; break;
            case	0x72:	{ BIT(ARG_1(code) , REG(D));                }; break;
            case	0x73:	{ BIT(ARG_1(code) , REG(E));                }; break;
            case	0x74:	{ BIT(ARG_1(code) , REG(H));                }; break;
            case	0x75:	{ BIT(ARG_1(code) , REG(L));                }; break;
            case	0x76:	{ uint8_t n = mem_read(REG(HL));
                                BIT(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x77:	{ BIT(ARG_1(code) , REG(A));				}; break;
            case	0x78:	{ BIT(ARG_1(code) , REG(B));                }; break;
            case	0x79:	{ BIT(ARG_1(code) , REG(C));                }; break;
            case	0x7A:	{ BIT(ARG_1(code) , REG(D));                }; break;
            case	0x7B:	{ BIT(ARG_1(code) , REG(E));                }; break;
            case	0x7C:	{ BIT(ARG_1(code) , REG(H));                }; break;
            case	0x7D:	{ BIT(ARG_1(code) , REG(L));                }; break;
            case	0x7E:	{ uint8_t n = mem_read(REG(HL));
                                BIT(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x7F:	{ BIT(ARG_1(code) , REG(A));				}; break;
            case	0x80:	{ RES(ARG_1(code) , REG(B));                }; break;
            case	0x81:	{ RES(ARG_1(code) , REG(C));                }; break;
            case	0x82:	{ RES(ARG_1(code) , REG(D));                }; break;
            case	0x83:	{ RES(ARG_1(code) , REG(E));                }; break;
            case	0x84:	{ RES(ARG_1(code) , REG(H));                }; break;
            case	0x85:	{ RES(ARG_1(code) , REG(L));                }; break;
            case	0x86:	{uint8_t n = mem_read(REG(HL));
                                RES(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x87:	{ RES(ARG_1(code) , REG(A));				}; break;
            case	0x88:	{ RES(ARG_1(code) , REG(B));                }; break;
            case	0x89:	{ RES(ARG_1(code) , REG(C));                }; break;
            case	0x8A:	{ RES(ARG_1(code) , REG(D));                }; break;
            case	0x8B:	{ RES(ARG_1(code) , REG(E));                }; break;
            case	0x8C:	{ RES(ARG_1(code) , REG(H));                }; break;
            case	0x8D:	{ RES(ARG_1(code) , REG(L));                }; break;
            case	0x8E:	{ uint8_t n = mem_read(REG(HL));
                                RES(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x8F:	{ RES(ARG_1(code) , REG(A));				}; break;
            case	0x90:	{ RES(ARG_1(code) , REG(B));                }; break;
            case	0x91:	{ RES(ARG_1(code) , REG(C));                }; break;
            case	0x92:	{ RES(ARG_1(code) , REG(D));                }; break;
            case	0x93:	{ RES(ARG_1(code) , REG(E));                }; break;
            case	0x94:	{ RES(ARG_1(code) , REG(H));                }; break;
            case	0x95:	{ RES(ARG_1(code) , REG(L));                }; break;
            case	0x96:	{ uint8_t n = mem_read(REG(HL));
                                RES(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x97:	{ RES(ARG_1(code) , REG(A));				}; break;
            case	0x98:	{ RES(ARG_1(code) , REG(B));                }; break;
            case	0x99:	{ RES(ARG_1(code) , REG(C));                }; break;
            case	0x9A:	{ RES(ARG_1(code) , REG(D));                }; break;
            case	0x9B:	{ RES(ARG_1(code) , REG(E));                }; break;
            case	0x9C:	{ RES(ARG_1(code) , REG(H));                }; break;
            case	0x9D:	{ RES(ARG_1(code) , REG(L));                }; break;
            case	0x9E:	{ uint8_t n = mem_read(REG(HL));
                                RES(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0x9F:	{ RES(ARG_1(code) , REG(A));				}; break;
            case	0xA0:	{ RES(ARG_1(code) , REG(B));                }; break;
            case	0xA1:	{ RES(ARG_1(code) , REG(C));                }; break;
            case	0xA2:	{ RES(ARG_1(code) , REG(D));                }; break;
            case	0xA3:	{ RES(ARG_1(code) , REG(E));                }; break;
            case	0xA4:	{ RES(ARG_1(code) , REG(H));                }; break;
            case	0xA5:	{ RES(ARG_1(code) , REG(L));                }; break;
            case	0xA6:	{ uint8_t n = mem_read(REG(HL));
                                RES(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xA7:	{ RES(ARG_1(code) , REG(A));				}; break;
            case	0xA8:	{ RES(ARG_1(code) , REG(B));                }; break;
            case	0xA9:	{ RES(ARG_1(code) , REG(C));                }; break;
            case	0xAA:	{ RES(ARG_1(code) , REG(D));                }; break;
            case	0xAB:	{ RES(ARG_1(code) , REG(E));                }; break;
            case	0xAC:	{ RES(ARG_1(code) , REG(H));                }; break;
            case	0xAD:	{ RES(ARG_1(code) , REG(L));                }; break;
            case	0xAE:	{ uint8_t n = mem_read(REG(HL));
                                RES(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xAF:	{ RES(ARG_1(code) , REG(A));				}; break;
            case	0xB0:	{ RES(ARG_1(code) , REG(B));                }; break;
            case	0xB1:	{ RES(ARG_1(code) , REG(C));                }; break;
            case	0xB2:	{ RES(ARG_1(code) , REG(D));                }; break;
            case	0xB3:	{ RES(ARG_1(code) , REG(E));                }; break;
            case	0xB4:	{ RES(ARG_1(code) , REG(H));                }; break;
            case	0xB5:	{ RES(ARG_1(code) , REG(L));                }; break;
            case	0xB6:	{ uint8_t n = mem_read(REG(HL));
                                RES(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xB7:	{ RES(ARG_1(code) , REG(A));				}; break;
            case	0xB8:	{ RES(ARG_1(code) , REG(B));                }; break;
            case	0xB9:	{ RES(ARG_1(code) , REG(C));                }; break;
            case	0xBA:	{ RES(ARG_1(code) , REG(D));                }; break;
            case	0xBB:	{ RES(ARG_1(code) , REG(E));                }; break;
            case	0xBC:	{ RES(ARG_1(code) , REG(H));                }; break;
            case	0xBD:	{ RES(ARG_1(code) , REG(L));                }; break;
            case	0xBE:	{ uint8_t n = mem_read(REG(HL));
                                RES(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xBF:	{ RES(ARG_1(code) , REG(A));				}; break;
            case	0xC0:	{ SET(ARG_1(code) , REG(B));                }; break;
            case	0xC1:	{ SET(ARG_1(code) , REG(C));                }; break;
            case	0xC2:	{ SET(ARG_1(code) , REG(D));                }; break;
            case	0xC3:	{ SET(ARG_1(code) , REG(E));                }; break;
            case	0xC4:	{ SET(ARG_1(code) , REG(H));                }; break;
            case	0xC5:	{ SET(ARG_1(code) , REG(L));                }; break;
            case	0xC6:	{ uint8_t n = mem_read(REG(HL));
                                SET(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xC7:	{ SET(ARG_1(code) , REG(A));				}; break;
            case	0xC8:	{ SET(ARG_1(code) , REG(B));                }; break;
            case	0xC9:	{ SET(ARG_1(code) , REG(C));                }; break;
            case	0xCA:	{ SET(ARG_1(code) , REG(D));                }; break;
            case	0xCB:	{ SET(ARG_1(code) , REG(E));                }; break;
            case	0xCC:	{ SET(ARG_1(code) , REG(H));                }; break;
            case	0xCD:	{ SET(ARG_1(code) , REG(L));                }; break;
            case	0xCE:	{ uint8_t n = mem_read(REG(HL));
                                SET(ARG_1(code) , n); 
                                mem_write(REG(HL), n);			}; break;
            case	0xCF:	{ SET(ARG_1(code) , REG(A));				}; break;
            case	0xD0:	{ SET(ARG_1(code) , REG(B));                }; break;
            case	0xD1:	{ SET(ARG_1(code) , REG(C));                }; break;
            case	0xD2:	{ SET(ARG_1(code) , REG(D));                }; break;
            case	0xD3:	{ SET(ARG_1(code) , REG(E));                }; break;
            case	0xD4:	{ SET(ARG_1(code) , REG(H));                }; break;
            case	0xD5:	{ SET(ARG_1(code) , REG(L));                }; break;
            case	0xD6:	{ uint8_t n = mem_read(REG(HL));
                                SET(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xD7:	{ SET(ARG_1(code) , REG(A));				}; break;
            case	0xD8:	{ SET(ARG_1(code) , REG(B));                }; break;
            case	0xD9:	{ SET(ARG_1(code) , REG(C));                }; break;
            case	0xDA:	{ SET(ARG_1(code) , REG(D));                }; break;
            case	0xDB:	{ SET(ARG_1(code) , REG(E));                }; break;
            case	0xDC:	{ SET(ARG_1(code) , REG(H));                }; break;
            case	0xDD:	{ SET(ARG_1(code) , REG(L));                }; break;
            case	0xDE:	{ uint8_t n = mem_read(REG(HL));
                                SET(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xDF:	{ SET(ARG_1(code) , REG(A));				}; break;
            case	0xE0:	{ SET(ARG_1(code) , REG(B));                }; break;
            case	0xE1:	{ SET(ARG_1(code) , REG(C));                }; break;
            case	0xE2:	{ SET(ARG_1(code) , REG(D));                }; break;
            case	0xE3:	{ SET(ARG_1(code) , REG(E));                }; break;
            case	0xE4:	{ SET(ARG_1(code) , REG(H));                }; break;
            case	0xE5:	{ SET(ARG_1(code) , REG(L));                }; break;
            case	0xE6:	{ uint8_t n = mem_read(REG(HL));
                                SET(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xE7:	{ SET(ARG_1(code) , REG(A));				}; break;
            case	0xE8:	{ SET(ARG_1(code) , REG(B));                }; break;
            case	0xE9:	{ SET(ARG_1(code) , REG(C));                }; break;
            case	0xEA:	{ SET(ARG_1(code) , REG(D));                }; break;
            case	0xEB:	{ SET(ARG_1(code) , REG(E));                }; break;
            case	0xEC:	{ SET(ARG_1(code) , REG(H));                }; break;
            case	0xED:	{ SET(ARG_1(code) , REG(L));                }; break;
            case	0xEE:	{ uint8_t n = mem_read(REG(HL));
                                SET(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xEF:	{ SET(ARG_1(code) , REG(A));				}; break;
            case	0xF0:	{ SET(ARG_1(code) , REG(B));                }; break;
            case	0xF1:	{ SET(ARG_1(code) , REG(C));                }; break;
            case	0xF2:	{ SET(ARG_1(code) , REG(D));                }; break;
            case	0xF3:	{ SET(ARG_1(code) , REG(E));                }; break;
            case	0xF4:	{ SET(ARG_1(code) , REG(H));                }; break;
            case	0xF5:	{ SET(ARG_1(code) , REG(L));                }; break;
            case	0xF6:	{ uint8_t n = mem_read(REG(HL));
                                SET(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xF7:	{ SET(ARG_1(code) , REG(A));				}; break;
            case	0xF8:	{ SET(ARG_1(code) , REG(B));                }; break;
            case	0xF9:	{ SET(ARG_1(code) , REG(C));                }; break;
            case	0xFA:	{ SET(ARG_1(code) , REG(D));                }; break;
            case	0xFB:	{ SET(ARG_1(code) , REG(E));                }; break;
            case	0xFC:	{ SET(ARG_1(code) , REG(H));                }; break;
            case	0xFD:	{ SET(ARG_1(code) , REG(L));                }; break;
            case	0xFE:	{ uint8_t n = mem_read(REG(HL));
                                SET(ARG_1(code) , n); 
                                mem_write(REG(HL), n); 			}; break;
            case	0xFF:	{ SET(ARG_1(code) , REG(A));				}; break;
            default:                      break;
        }

    }

    cpu.clock += cpu.currentCycleLength;
    

    cpu_updateTimer();


}



void cpu_updateTimer()
{
    cpu._DIVhelper += cpu.currentCycleLength;
    // TODO in double speed mode
    uint16_t divider = 64;
    if ( cpu._DIVhelper >= divider)
    {
        cpu._DIVhelper -= divider;
        mem.DIV++;
    }

    if (mem.TAC & ( 1<<2 )) // if counter is enabled
    {
        cpu._TIMAhelper += cpu.currentCycleLength;
        uint8_t preOverflow = mem.TIMA == 0xFF ? 1 : 0;
        uint16_t TIMAdivider; // get divider. in this case its already divides by 4 because i only count cpu steps, not the real clock
        switch (mem.TAC & 0x3)
        {
            case 0:
                TIMAdivider = 256;
                break;
            case 1:
                TIMAdivider = 4;
                break;
            case 2:
                TIMAdivider = 16;
                break;
            case 3:
                TIMAdivider = 64;
                break;
        }

        if (cpu._TIMAhelper >= TIMAdivider)
        {
            mem.TIMA++;
            cpu._TIMAhelper -= TIMAdivider;
        }

        if (preOverflow && mem.TIMA == 0x00) // overflow happened
        {
            mem.TIMA = mem.TMA;
            TRIGGER_IRQ(IRQ_TIMER);
        }

    }


}










