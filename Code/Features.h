namespace Features {
	void UpdatePerTick();
	extern bool BlindEyeAll;
	extern bool FixLoop;
	extern bool FreezeAll;
	extern bool WaterAll;
	extern bool FireAll;
	extern bool SoundSpamAll;
	extern bool RockstarCheck;
	extern bool ExplodeAll;
	extern bool NeverWantedAll;
	extern bool RemoteOTRAll;
	extern bool CameraSpamAll;
	extern bool SpoofLevel;
	void SpoofLevels(int value);
	void modelswap();
	void SendTextMessage(char* msg);
	void RockstarAdminCheck();
	void HalloweenEnable();
	void SetWeathers(int weather);
	void SendTextMessage(int player);
	void GodMode();
	void LoadPlugins(fs::path asi_path);
	extern bool GodModeBool;

	extern int SpoofLevelsAmount;
	extern bool ShowIPs;

	extern std::string ConsoleLogger;

	extern bool NeverWantedBool;
	void NeverGetWanted();

	extern bool espbool;
	void esp();

	extern bool HeaderImage;
	extern bool dowbool;
	extern bool Forcefield;
	extern bool RemoteNeverWanted[32];
	extern bool ShootVehicle;
	extern bool DisplayFPS;
	extern bool CashDropAll;
	void featureDisplayFPS();
	void shootveh();
	extern bool CashDropLocationBool;
	extern bool BlockAllProtex;
	extern bool ChatCommands, SlashVehicle, SlashCash, SlashOtr, SlashDay, SlashNight, SlashSnow, SlashClear, SlashWeapons, SlashNeverWanted, SlashClone;

	void OfficeMoney();
	void OfficeStatue();
	void CashDropLocation();
	void DriveOnWater();
	void SaveOutfit(std::string name);
	extern bool ObjectGunBool;
	extern bool VALKYIREROCKET;
	void valkyirerocket();
	extern bool GravityGun;
	void gravitygun();
	void objectgunpl();
	extern bool AntiMoneyDrop;
	void Enemy(char* model);
	void Bodyguard(char* model);
	extern bool ToBank;
	extern bool SlowMoBool;
	void SlowMo();

	void DeleteVeh();

	extern bool SpawnMaxed;
	extern bool SpawnInCar;

	extern bool freecambool;
	void freecam();

	extern bool SuperJumpBool;
	void SuperJump();

	extern bool AirWalkBool;
	void AirWalk();

	extern bool AFKNo;
	void AntiAFK();

	extern bool RunningBool;
	void RunningFast();

	extern bool BackstrokePro;
	void BackstrokeSpeed();

	extern bool InvisibilityBool;
	void Invisibility();

	extern bool mobileRadio;
	void mobiletunes();

	extern bool RagdollBool;
	void NoRagdoll();

	extern bool OffRadarBool;
	void OffRadar();

	extern bool TinyPlayerBool;
	void TinyPlayer();

	extern int WantedLevel;
	void ChangeWanted(int level);

	extern std::string String;
	void SetName(const std::string& name);

	void HealPlayer();

	extern bool SpectateList[32];
	void Spectate(int target);

	extern bool FuckCamBool;
	void FuckTheirCam();

	extern bool ExplodeList[32];
	void ExplodeLoopPlayer(int target);

	extern bool CashDropRemote[32];
	void CashDropV2(int target);

	extern bool WaterList[32];
	void WaterLoop(int target);

	extern bool FireList[32];
	void FireLoop(int target);

	extern bool FreezeList[32];
	void Freeze(int target);

	void TeleportToPlayer(int player);

	void TrapPlayer(int ped);

	void BlamePlayer(int source);

	void AirStrikeKill();

	void RemoteLevels(int selectedPlayer);

	void CloneVehicle(int ped);

	void TeleportInVehicle(int player);

	void KickFromVehicle(int player);

	void RamPlayer();

	void AttachObjects(char* object);

	void Teleport1(bool all = false);
	void Teleport2(bool all = false);
	void Teleport3(bool all = false);
	void Teleport4(bool all = false);
	void Teleport5(bool all = false);
	void Teleport6(bool all = false);
	void Teleport7(bool all = false);
	void Teleport8(bool all = false);
	void Teleport9(bool all = false);
	void Teleport10(bool all = false);
	void Teleport11(bool all = false);
	void Teleport12(bool all = false);
	void Teleport13(bool all = false);
	void Teleport14(bool all = false);
	void Teleport15(bool all = false);
	void Teleport16(bool all = false);
	void Teleport17(bool all = false);
	void Teleport18(bool all = false);
	void Teleport19(bool all = false);
	void Teleport20(bool all = false);
	void TeleportCustom(int place);

	extern bool Spam[32];
	void DoSoundSpam(int player, bool all = false);

	extern bool RemoteOTRBool[32];
	void RemoteOTR(int target);

	void ForceDisappear();

	void RemoveWanted(int player);

	void SendToJob(bool all = false);

	void KickPlayer(bool all = false);

	void CEOKick(bool all = false);

	void CEOBan(bool all = false);

	void InfiniteLoading();

	void TrollMessage();

	void Invite2Place(int player, int place);

	void MessageSpam(int player, std::int64_t message);

	extern bool InfiniteAmmoBool;
	void InfiniteAmmo();

	extern bool ExplosiveAmmoFlag;
	void ExplosiveAmmo();

	extern bool FireShoot;
	void ShootFire();

	extern bool WaterShoot;
	void ShootWater();

	extern bool FireworkBool;
	void FireWork();

	extern bool FireworkBool;
	void FireWork();

	extern bool AirStrike;
	void PreditorGun();

	extern bool RainbowWeaponsBool;
	void RainbowWeapons();

	extern bool AimbotBool;
	void Aimbot();

	extern bool DamageBool;
	void DamageMod();

	extern bool Recoil;
	void NoRecoil();

	extern bool RapidFireBool;
	void RapidFire();

	extern bool OrbitalCooldownBool;
	void OrbitalCooldown();

	extern int boostlevel;
	void carboost();

	void PlateEdit(char* plate);
	extern std::vector<std::string> list;
	extern std::vector<std::string> ListOutfit;
	extern bool cargodmodebool;
	void cargodmode();

	extern bool Boost;
	void RocketBoost();

	extern bool InvisibleCarBool;
	void InvisibleCar();

	extern bool vehiclegravitybool;
	void vehiclegravity();

	extern bool WeaponRockets;
	void VehicleRockets();

	extern bool SpeedoBool;
	extern bool MPH;
	extern bool PlateMPH;
	void Speedometer();

	extern bool MatrixPlates;
	void MatrixPlate();

	extern bool StickyBool;
	void Sticky();

	void FixCar();

	void maxvehicle();

	void OpenDoors();

	void flip();

	extern int primarycolorintr;
	extern int primarycolorintg;
	extern int primarycolorintb;
	void PrimaryColor();

	extern int secondarycolorintr;
	extern int secondarycolorintg;
	extern int secondarycolorintb;
	void SecondaryColor1();

	extern bool rainbowbool;
	void rainbow();

	extern bool RainbowPaintLoop;
	void HasPaintLoop();

	void LoadPreset(std::string name);
	extern std::vector<std::string> listPresets();
	extern std::vector<std::string> listoutfit();
	void LoadOutfit(std::string name);

	void AltStealth(int amount);
	extern bool moneyrain2k;
	void RainMoney();
	extern bool superman;
	extern bool EXPLOSION_EVENT;
	extern bool GAME_CLOCK_EVENT;
	extern bool GAME_WEATHER_EVENT;
	extern bool NETWORK_CLEAR_PED_TASKS_EVENT;
	extern bool GIVE_WEAPON_EVENT;
	extern bool REMOVE_WEAPON_EVENT;
	extern bool REMOVE_ALL_WEAPONS_EVENT;
	extern bool FIRE_EVENT;
	extern bool ALTER_WANTED_LEVEL_EVENT;
	void TimeSpam();
	extern bool TimeSpamBool;
	void SuperMan();
	extern int TimePD1;
	extern int BagDelay;
	extern bool AimBool;
	void AimToDelete();
	extern int TimeHour;
	extern int TimeMinute;
	void UpdatePresence();

	extern int TimeSecond;
	void settime(int Hours, int Minutes, int Seconds);

	void Yankton();
	void Yacht();
	void Carrier();
	void SunkenShip();
	void PrimaryColor();
	void Hospital();
	void Farm();
	void Morgue();
	void FixCar();
	void Invader();
	void Jewelry();

	void IrlTime();

	extern int WaveIntensityValue;
	void SetWaveIntensity();

	void SetWeather(int weather);

	extern bool GravityBool;
	void Gravity();

	extern bool explodenearbyvehiclesbool;
	void explodenearbyvehicles();

	extern int Levels[8000];
	extern int levelrp;
	void SetRank(int rpvalue);

	extern bool shootcashbool;
	void shootcash();

	extern bool teleportbool;
	void teleportgun();

	extern bool savenewdrop;
	void cashdropself();

	void doStealth(int amount);
	void RemoveStealth(int amount);
	extern bool StealthLoop;

	void UnlockAll();
	void UnlockSnacks();
	void UnlockArmor();
	void UnlockFireworks();
	void UnlockStats();
	void UnlockTattoos();
	void UnlockParachutes();
	void UnlockRims();
	void UnlockVehicles();
	void UnlockAchievements();
	void UnlockHairstyles();
	void UnlockWeapons();
	void UnlockClothes();

	extern bool  SnowBool;
	extern bool  SnowBool2;
	extern bool  XmasShitBool;
	extern bool  HalloweenShit;

	extern bool  ValentineClothingBool;
	extern bool  ValentineVehicleBool;
	extern bool  ValentineGunBool;
	extern bool  ValentineMaskBool;
	extern bool  ValentineHairBool;
	extern bool  HaloweenWeaponsBool;
	extern bool  HeistBoxvilleBool;
	extern bool  HeistEnduroBool;
	extern bool  HeistHydraBool;
	extern bool  HeistInsurgentBool;
	extern bool  HeistInsurgent2Bool;
	extern bool  HeistLectroBool;
	extern bool  HeistMuleBool;
	extern bool  HeistSavageBool;
	extern bool  HeistTechnicalBool;
	extern bool  HeistValkyrieBool;
	extern bool  HeistPrisonBool;
	extern bool  SnowballBool;
	extern bool  ChristmasContentBool;
	extern bool  ChristmasClothingBool;
	extern bool  ChristmasMaskBool;
	extern bool  ChristmasVehicleBool;
	extern bool  IndpendenceDayBool;
	extern bool  WesternSoverignBool;
	extern bool  MonsterTruckBool;
	extern bool  BeerHat1Bool;
	extern bool  BeerHat2Bool;
	extern bool  BeerHat3Bool;
	extern bool  BeerHat4Bool;
	extern bool  BeerHat5Bool;
	extern bool  BeerHat6Bool;
	void BeerHats();
	void LoadPlayerInfo(int player);
	extern std::string Token;

	extern bool Nametag;
	void NameTagESP();

	extern int StealthAmount;

	extern bool LogEvent;
	extern bool DisableCell;
	void DisablePhone();
	extern bool ChatSensor;
	extern bool LogMetric;
	extern bool EventNotif;

	void ClearProps();

	extern bool Hidden;

	void BruteForceWeaponAddons(Ped ped, Hash weaponHash);

	void TransactionError(int player);

	extern bool MentalStateBool;
	void MentalState();

	void GentleKick(bool all = false);


	extern bool BlindEyeBool[32];
	void BlindEye(int player);

	void BannerMessage();

	extern bool ToggledOn;
	extern bool ToggledOff;
	void SetLobbyTime(int hr, int min, int sec);
	void SetWanted(int level);
	void BankedMoney();
	extern bool AddSpam;
	extern bool BannerSpam;
	extern bool RemoveSpam;

	extern bool CameraSpam[32];
	void sendFaceCameraForward(int player);

	void StolenMoney();

	extern bool DevMode;

	extern bool AmmoEditorBool;
	void AmmoEditor(std::string weaponType);
}