
# -*- coding: utf-8 -*-

"""
Device
^^^^^^
This package provides a top-level device class, ``RM_Device_EFR32XG25XFULL_RevA1``,
which is instantiated in either an offline mode for stand-alone simulated
register I/O or in a live session with a real part.

  .. autoclass::  RM_Device_EFR32XG25XFULL_RevA1
     :members:
     :show-inheritance:
     :inherited-members:

The ARM CMSIS System View Description (SVD) XML hierarchy is reflected in
this package module composition. The ``device.py`` imports a number of
peripheral modules, which in turn import register, field, and optional enum
modules.

"""


__all__ = [ 'RM_Device_EFR32XG25XFULL_RevA1' ]

from . static import Base_RM_Device, RM_SVD_Info
from .... common import RM_TrustZone_Access_Mode
from . AGC_NS import *
from . AGC_S import *
from . CW import *
from . FEFILT0_NS import *
from . FEFILT0_S import *
from . FEFILT1_NS import *
from . FEFILT1_S import *
from . FRC_NS import *
from . FRC_S import *
from . LEGOQPSK import *
from . LRSWKOFDM import *
from . MODEM_NS import *
from . MODEM_S import *
from . RAC_NS import *
from . RAC_S import *
from . RFCRC_NS import *
from . RFCRC_S import *
from . RFFPLL0_NS import *
from . RFFPLL0_S import *
from . SEQ import *
from . SMCTRL_NS import *
from . SMCTRL_S import *
from . SUNFSK import *
from . SUNOFDM import *
from . SUNOQPSK import *
from . SYNTH_NS import *
from . SYNTH_S import *
from . TXFRONT_NS import *
from . TXFRONT_S import *

import imp
import os

_PKG_PATH = os.path.dirname(os.path.realpath(__file__))
_EXCLUDED_REGS_MOD_NAME = 'excluded_regs'
_EXCLUDED_REGS_LIST_NAME = 'EXCLUDED_REGS'


class RM_Device_EFR32XG25XFULL_RevA1(Base_RM_Device):
    """
    The top level register map class. This class can be used in an offline
    mode for stand-alone simulated register I/O. In this case the class will
    use a dictionary initialized with reset values from the CMSIS SVD XML.

    .. code-block:: py

        # Example: Use register map package in offline mode
        import sys
        sys.path.append("path/to/this/package")

        from host_py_rm_studio_internal import RM_Factory
        rm_device_class = RM_Factory("JUMBO", "A0")
        rm = rm_device_class(label='offline_rm')

    The class is used by the base application classes in the Python Debug Bench
    to control the part. This requires an RPC interface provided by the
    pyvsrpc package. If you want to use live outside the PDB, you can use the
    JLINK_AccessManager class from the pyjlinklib package.

    .. code-block:: py

        # Example: Use register map with live J-Link connection
        import sys
        sys.path.append("path/to/the/packages")

        from pyrmsvd import *
        from pyjlinklib import *
        from host_py_rm_studio_internal import RM_Factory

        dut_label = 'dut_1'
        accessMgr = JLINK_AccessManager(dut_label, JLINK_ARM_OPTIONS(), False)
        accessMgr.Connect()

        # auto detect part and request corresponding RM class
        hwinfo = EFRDetectPlatform(self.accessMgr)
        rm_device_class = RM_Factory(hwinfo.family_name, hwinfo.rev_name)
        rmIO = RegisterMapInterface(accessMgr.ReadRegister,
                                    accessMgr.WriteRegister)
        rm = rm_device_class(rmIO, dut_label)

    The ``svdInfo`` property is an instance of :class:`pyrmsvd.static.common.svdinfo.RM_SVD_Info`.

    """

    def __init__(self, rmio=None, label='offline_rm', default_tz_access_mode=RM_TrustZone_Access_Mode.TZ_SECURE):
        """
        Register the ``RegisterMapInterface`` instance and the logging label.

        :type  rmio: :class:`pyrmsvd.static.common.regmapio.RegisterMapInterface` or ``NoneType``
        :param rmio: The register map I/O instance, which contains the \
                     read/write functions for the registers. If ``None``, then \
                     the class will automatically create an internal ``RegisterMapInterface`` \
                     with an :class:`pyrmsvd.static.common.accessmgr.Offline_AccessManager` \
                     instance for operation in offline mode.
        :type  label: ``str``
        :param label: The register map DUT label to use in the system logger.
        :param default_tz_access_mode: Default ARM v8m Trustzone access mode to be used \
        			for non-suffixed peripherals ('S' = Secure; 'NS' = Non-Secure). This \
        			option only applies to Series 2/3 devices with Cortex M33 core
        """
        assert isinstance(default_tz_access_mode, RM_TrustZone_Access_Mode)
        self.__dict__['zz_frozen'] = False
        super(RM_Device_EFR32XG25XFULL_RevA1, self).__init__(rmio, label,
            'EFR32XG25XFULL',
            RM_SVD_Info('EFR32XG25XFULL_SEQ.svd', '4e8d938d440dea4b357d12057a9e9bc3'))

        self.AGC_NS = RM_Peripheral_AGC_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['AGC_NS'] = self.AGC_NS
        self.AGC_S = RM_Peripheral_AGC_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['AGC_S'] = self.AGC_S
        self.CW = RM_Peripheral_CW(self.zz_rmio, self.zz_label)
        self.zz_pdict['CW'] = self.CW
        self.FEFILT0_NS = RM_Peripheral_FEFILT0_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['FEFILT0_NS'] = self.FEFILT0_NS
        self.FEFILT0_S = RM_Peripheral_FEFILT0_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['FEFILT0_S'] = self.FEFILT0_S
        self.FEFILT1_NS = RM_Peripheral_FEFILT1_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['FEFILT1_NS'] = self.FEFILT1_NS
        self.FEFILT1_S = RM_Peripheral_FEFILT1_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['FEFILT1_S'] = self.FEFILT1_S
        self.FRC_NS = RM_Peripheral_FRC_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['FRC_NS'] = self.FRC_NS
        self.FRC_S = RM_Peripheral_FRC_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['FRC_S'] = self.FRC_S
        self.LEGOQPSK = RM_Peripheral_LEGOQPSK(self.zz_rmio, self.zz_label)
        self.zz_pdict['LEGOQPSK'] = self.LEGOQPSK
        self.LRSWKOFDM = RM_Peripheral_LRSWKOFDM(self.zz_rmio, self.zz_label)
        self.zz_pdict['LRSWKOFDM'] = self.LRSWKOFDM
        self.MODEM_NS = RM_Peripheral_MODEM_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['MODEM_NS'] = self.MODEM_NS
        self.MODEM_S = RM_Peripheral_MODEM_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['MODEM_S'] = self.MODEM_S
        self.RAC_NS = RM_Peripheral_RAC_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['RAC_NS'] = self.RAC_NS
        self.RAC_S = RM_Peripheral_RAC_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['RAC_S'] = self.RAC_S
        self.RFCRC_NS = RM_Peripheral_RFCRC_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['RFCRC_NS'] = self.RFCRC_NS
        self.RFCRC_S = RM_Peripheral_RFCRC_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['RFCRC_S'] = self.RFCRC_S
        self.RFFPLL0_NS = RM_Peripheral_RFFPLL0_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['RFFPLL0_NS'] = self.RFFPLL0_NS
        self.RFFPLL0_S = RM_Peripheral_RFFPLL0_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['RFFPLL0_S'] = self.RFFPLL0_S
        self.SEQ = RM_Peripheral_SEQ(self.zz_rmio, self.zz_label)
        self.zz_pdict['SEQ'] = self.SEQ
        self.SMCTRL_NS = RM_Peripheral_SMCTRL_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['SMCTRL_NS'] = self.SMCTRL_NS
        self.SMCTRL_S = RM_Peripheral_SMCTRL_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['SMCTRL_S'] = self.SMCTRL_S
        self.SUNFSK = RM_Peripheral_SUNFSK(self.zz_rmio, self.zz_label)
        self.zz_pdict['SUNFSK'] = self.SUNFSK
        self.SUNOFDM = RM_Peripheral_SUNOFDM(self.zz_rmio, self.zz_label)
        self.zz_pdict['SUNOFDM'] = self.SUNOFDM
        self.SUNOQPSK = RM_Peripheral_SUNOQPSK(self.zz_rmio, self.zz_label)
        self.zz_pdict['SUNOQPSK'] = self.SUNOQPSK
        self.SYNTH_NS = RM_Peripheral_SYNTH_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['SYNTH_NS'] = self.SYNTH_NS
        self.SYNTH_S = RM_Peripheral_SYNTH_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['SYNTH_S'] = self.SYNTH_S
        self.TXFRONT_NS = RM_Peripheral_TXFRONT_NS(self.zz_rmio, self.zz_label)
        self.zz_pdict['TXFRONT_NS'] = self.TXFRONT_NS
        self.TXFRONT_S = RM_Peripheral_TXFRONT_S(self.zz_rmio, self.zz_label)
        self.zz_pdict['TXFRONT_S'] = self.TXFRONT_S

        if default_tz_access_mode == RM_TrustZone_Access_Mode.TZ_SECURE:
            self.AGC = self.AGC_S
            self.FEFILT0 = self.FEFILT0_S
            self.FEFILT1 = self.FEFILT1_S
            self.FRC = self.FRC_S
            self.MODEM = self.MODEM_S
            self.RAC = self.RAC_S
            self.RFCRC = self.RFCRC_S
            self.RFFPLL0 = self.RFFPLL0_S
            self.SMCTRL = self.SMCTRL_S
            self.SYNTH = self.SYNTH_S
            self.TXFRONT = self.TXFRONT_S
            pass
        elif default_tz_access_mode == RM_TrustZone_Access_Mode.TZ_NONSECURE:
            self.AGC = self.AGC_NS
            self.FEFILT0 = self.FEFILT0_NS
            self.FEFILT1 = self.FEFILT1_NS
            self.FRC = self.FRC_NS
            self.MODEM = self.MODEM_NS
            self.RAC = self.RAC_NS
            self.RFCRC = self.RFCRC_NS
            self.RFFPLL0 = self.RFFPLL0_NS
            self.SMCTRL = self.SMCTRL_NS
            self.SYNTH = self.SYNTH_NS
            self.TXFRONT = self.TXFRONT_NS
            pass

        if self.offline:
            # assign the default reset values to the simulated register dictionary
            for key in self.zz_pdict:
                self.zz_pdict[key].assignRegDefault()
        # build the register address-to-names mapping
        for pkey in self.zz_pdict:
            per_obj = self.zz_pdict[pkey]
            # TODO: Remove deprecated self.zz_reg_addr_to_name in future pyrmsvd 2.x release
            per_obj.getAddressNameMap(self.zz_reg_addr_to_name)
            per_obj.getAddressNamesMap(self.zz_reg_addr_to_names)
            if per_obj.isAliased():
                # register device-level method to query the self.zz_reg_addr_to_names dict
                per_obj.registerAddressToNamesCb(self.addressToNames)

        self.__dict__['zz_frozen'] = True

        # Process an optional exclusion list that will remove a register
        # from a unfiltered register dump operation performed in the dump()
        # method. The register is also removed from the generated list in
        # the buildRegFilterList() method. Requires pyrmsvd support, so
        # check if this version of the base class supports register exclusion.

        excluded_regs_fn = os.path.join(_PKG_PATH, _EXCLUDED_REGS_MOD_NAME + '.py')
        if os.path.exists(excluded_regs_fn) and hasattr(self, 'excludeFromDumpByName'):
            mod = imp.load_source(_EXCLUDED_REGS_MOD_NAME, excluded_regs_fn)
            excluded_regs = getattr(mod, _EXCLUDED_REGS_LIST_NAME)
            self.excludeFromDumpByName(excluded_regs)